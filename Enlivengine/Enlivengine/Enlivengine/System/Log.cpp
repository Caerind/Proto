#include <Enlivengine/System/Log.hpp>

#ifdef ENLIVE_ENABLE_LOG

#include <bitset>

#if defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)
#include <Windows.h>
#endif // defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)

#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/System/String.hpp>

namespace en
{

#ifdef ENLIVE_ENABLE_DEFAULT_LOGGER
ConsoleLogger ConsoleLogger::sConsoleLogger;
#if defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)
VisualStudioLogger VisualStudioLogger::sVisualStudioLogger;
#endif // defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)
#endif // ENLIVE_ENABLE_DEFAULT_LOGGER

std::string_view LogMessage::GetTypeString() const
{
	return Meta::GetEnumName(type);
}

std::string_view LogMessage::GetChannelString() const
{
	return LogManager::GetInstance().GetChannelString(channel);
}

std::string_view LogMessage::GetMessageString() const
{
	return std::string_view(message);
}

Logger::Logger()
	: mTypeFilter(static_cast<U32>(LogType::All))
	, mChannelFilter(static_cast<U64>(LogChannel::All))
	, mEnabled(true)
{
	RegisterLogger();
}

Logger::~Logger()
{
	UnregisterLogger();
}

void Logger::Enable(bool enable)
{
	mEnabled = enable;
}

bool Logger::IsEnabled() const
{
	return mEnabled;
}

void Logger::SetTypeFilter(U32 typeFilter)
{
	mTypeFilter = typeFilter;
}

U32 Logger::GetTypeFilter() const
{
	return mTypeFilter;
}

void Logger::SetChannelFilter(U64 channelFilter)
{
	mChannelFilter = channelFilter;
}

U64 Logger::GetChannelFilter() const
{
	return mChannelFilter;
}

bool Logger::PassFilters(LogType type, U64 channelID) const
{
	return (channelID & mChannelFilter) != 0 && (static_cast<U32>(type) & mTypeFilter) != 0;
}

void Logger::RegisterLogger()
{
	LogManager::GetInstance().RegisterLogger(this);
}

void Logger::UnregisterLogger()
{
	LogManager::GetInstance().UnregisterLogger(this);
}

LogManager::LogManager()
	: mLoggers()
	, mDefaultLogger(nullptr)
	, mTypeFilter(static_cast<U32>(LogType::All))
	, mChannelFilter(static_cast<U64>(LogChannel::All))
{
}

void LogManager::SetTypeFilter(U32 typeFilter)
{
	mTypeFilter = typeFilter;
}

U32 LogManager::GetTypeFilter() const
{
	return mTypeFilter;
}

void LogManager::SetChannelFilter(U64 channelFilter)
{
	mChannelFilter = channelFilter;
}

U64 LogManager::GetChannelFilter() const
{
	return mChannelFilter;
}

bool LogManager::PassFilters(LogType type, U64 channelID) const
{
	return (channelID & mChannelFilter) != 0 && (static_cast<U32>(type)& mTypeFilter) != 0;
}

U32 LogManager::GetLoggerCount() const
{
	return static_cast<U32>(mLoggers.size());
}

std::string_view LogManager::GetChannelString(U64 channelID) const
{
	enAssert(std::bitset<64>(channelID).count() == 1);
	if (channelID > static_cast<U64>(LogChannel::AllEngine))
	{
		U32 index = 0;
		U64 channelTest = static_cast<U64>(LogChannel::AllEngine) + 1;
		do 
		{
			if (channelTest == channelID)
			{
				enAssert(index < static_cast<U32>(mUserChannelStrings.size()));
				return mUserChannelStrings[index];
			}
			else
			{
				channelTest *= 2;
				index++;
			}
		} while (channelTest <= channelID);
		enAssert(false);
		return std::string_view("<UnknownChannel>");
	}
	else
	{
		return Meta::GetEnumName(static_cast<LogChannel>(channelID));
	}
}

void LogManager::RegisterLogger(Logger* logger)
{
	if (logger != nullptr)
	{
		mLoggers.push_back(logger);
	}
}

void LogManager::UnregisterLogger(Logger* logger)
{
	const U32 size = GetLoggerCount();
	for (U32 i = 0; i < size; i++)
	{
		if (mLoggers[i] == logger)
		{
			mLoggers.erase(mLoggers.begin() + i);
			return;
		}
	}
}

bool LogManager::IsRegistered(const Logger* logger) const
{
	const U32 size = GetLoggerCount();
	for (U32 i = 0; i < size; i++)
	{
		if (mLoggers[i] == logger)
		{
			return true;
		}
	}
	return false;
}

ConsoleLogger::ConsoleLogger()
	: Logger()
{
}

ConsoleLogger::~ConsoleLogger()
{
	UnregisterLogger();
}

void ConsoleLogger::Write(const LogMessage& message)
{
#ifdef ENLIVE_PLATFORM_WINDOWS
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (message.type)
	{
	case LogType::Warning: 
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case LogType::Error:
	case LogType::Fatal: 
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
		break;
	case LogType::Info: 
	default:
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	}

	fmt::print("[{}][{}] {}\n",
		message.GetTypeString(),
		message.GetChannelString(),
		message.GetMessageString());

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif // ENLIVE_PLATFORM_WINDOWS

#ifdef ENLIVE_PLATFORM_UNIX
	char colorChar;
	switch (message.type)
	{
	case LogType::Warning:
		colorChar = '\e[34m';
		break;
	case LogType::Error:
	case LogType::Fatal:
		colorChar = '\e[31m';
		break;
	case LogType::Info:
	default:
		colorChar = '\e[0m';
		break;
	}
	fmt::print("{}[{}][{}] {}\e[0m\n",
		message.GetTypeString(),
		message.GetChannelString(),
		message.GetMessageString());
#endif // ENLIVE_PLATFORM_UNIX
}

FileLogger::FileLogger(const std::string& filename)
	: Logger()
{
	SetFilename(filename);
}

FileLogger::~FileLogger()
{
	UnregisterLogger();
}

void FileLogger::SetFilename(const std::string& filename)
{
	if (filename.size() > 0)
	{
		if (mFile.is_open())
		{
			mFile.close();
		}

		mFile.open(filename);

		if (mFile.is_open())
		{
			mFilename = filename;
		}
		else
		{
			mFilename.clear();
		}
	}
}

const std::string& FileLogger::GetFilename() const
{
	return mFilename;
}

void FileLogger::Write(const LogMessage& message)
{
	if (mFile.is_open())
	{
		mFile << "[" << message.GetTypeString() << "]";
		mFile << "[" << message.GetChannelString() << "]";
		mFile << " " << message.GetMessageString() << '\n';
		mFile.flush();
	}
}

#if defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)
VisualStudioLogger::VisualStudioLogger()
	: Logger()
{
}

VisualStudioLogger::~VisualStudioLogger()
{
	UnregisterLogger();
}

void VisualStudioLogger::Write(const LogMessage& message)
{
	const std::string s = fmt::format("[{}][{}] {}\n",
		message.GetTypeString(),
		message.GetChannelString(),
		message.GetMessageString());
	OutputDebugStringA(s.c_str());
}

#endif // defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)

} // namespace en

#endif // ENLIVE_ENABLE_LOG