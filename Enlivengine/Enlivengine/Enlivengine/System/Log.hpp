#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#ifdef ENLIVE_ENABLE_LOG

#include <fstream>
#include <vector>
#include <string>

#include <fmt/format.h>

#include <Enlivengine/System/Singleton.hpp>
#include <Enlivengine/System/MetaEnum.hpp>

namespace en
{

enum class LogType : U32
{
	None = 0,
	Info = (1 << 1),
	Warning = (1 << 2),
	Error = (1 << 3),
	Fatal = (1 << 4),
	All = U32_Max
};

enum class LogChannel : U64
{
	None = 0,
	Global = (1 << 0),
	System = (1 << 1),
	Math = (1 << 2),
	Application = (1 << 3),
	Graphics = (1 << 4),
	Map = (1 << 5),
	Animation = (1 << 6),

	// TODO : Add others

	AllEngine = (1 << 7) - 1,
	All = U64_Max,
	AllClient = All - AllEngine
};

/*
Register your own LogChannels :
'x' must follow last engine channel

enum class LogChannelClient : en::U64
{
	GameplayChannel1 = (1 << x),
	GameplayChannel2 = (1 << x+1),
	...
};

Then use LogManager::InitializeClientChannels<LogChannelClient>() in your main and check that the returned value is true :)
*/

struct LogMessage
{
	std::string message; // TODO : Might this be transformed to string_view ?
	U64 channel; 
	LogType type;

	std::string_view GetTypeString() const;
	std::string_view GetChannelString() const;
	std::string_view GetMessageString() const;
};

class Logger
{
public:
	Logger();
	virtual ~Logger();

	void Enable(bool enable);
	bool IsEnabled() const;

	void SetTypeFilter(U32 typeFilter);
	U32 GetTypeFilter() const;

	void SetChannelFilter(U64 channelFilter);
	U64 GetChannelFilter() const;

	bool PassFilters(LogType type, U64 channelID) const;

	virtual void Write(const LogMessage& message) = 0;

protected:
	void RegisterLogger();
	void UnregisterLogger();

private:
	U64 mChannelFilter;
	U32 mTypeFilter;
	bool mEnabled;
};

class LogManager
{
	ENLIVE_SINGLETON(LogManager);

public:
	template <typename... Args>
	void Write(LogType type, U64 channelID, std::string_view formatStr, Args&& ...args);

	void SetTypeFilter(U32 typeFilter);
	U32 GetTypeFilter() const;

	void SetChannelFilter(U64 channelFilter);
	U64 GetChannelFilter() const;

	bool PassFilters(LogType type, U64 channelID) const;

	U32 GetLoggerCount() const;

	template <typename EnumClient>
	bool InitializeClientChannels();
	std::string_view GetChannelString(U64 channelID) const;

private:
	friend class Logger;
	void RegisterLogger(Logger* logger);
	void UnregisterLogger(Logger* logger);
	bool IsRegistered(const Logger* logger) const;

private:
	std::vector<Logger*> mLoggers;
	std::vector<std::string_view> mUserChannelStrings;
	Logger* mDefaultLogger;
	U32 mTypeFilter;
	U64 mChannelFilter;
};

template <typename... Args>
void LogManager::Write(LogType type, U64 channelID, std::string_view formatStr, Args&& ...args)
{
	if (PassFilters(type, channelID))
	{
		LogMessage message;
		message.message = fmt::format(formatStr, std::forward<Args>(args)...);
		message.type = type;
		message.channel = channelID;

		for (const auto& logger : mLoggers)
		{
			if (logger->IsEnabled() && logger->PassFilters(type, channelID))
			{
				logger->Write(message);
			}
		}
	}
}

template <typename EnumClient>
bool LogManager::InitializeClientChannels()
{
	const auto enumValues = magic_enum::enum_values<EnumClient>();
	mUserChannelStrings.clear();
	mUserChannelStrings.reserve(enumValues.size());
	bool valid = enumValues.size() > 0;
	U64 nextChannelID = 0;
	for (const auto& enumValue : enumValues)
	{
		if (valid)
		{
			if (nextChannelID == 0)
			{
				valid = (static_cast<U64>(enumValue) == static_cast<U64>(LogChannel::AllEngine) + 1);
				nextChannelID = static_cast<U64>(enumValue) * 2;
			}
			else
			{
				valid = (static_cast<U64>(enumValue) == nextChannelID);
				nextChannelID *= 2;
			}

			if (valid)
			{
				mUserChannelStrings.push_back(Meta::GetEnumName(enumValue));
			}
		}
	}
	if (!valid)
	{
		mUserChannelStrings.clear();
	}
	return valid;
}

class ConsoleLogger : public Logger
{
public:
	ConsoleLogger();
	virtual ~ConsoleLogger();

	virtual void Write(const LogMessage& message);
};

class FileLogger : public Logger
{
public:
	FileLogger(const std::string& filename = "");
	virtual ~FileLogger();

	void SetFilename(const std::string& filename);
	const std::string& GetFilename() const;

	virtual void Write(const LogMessage& message);

private:
	std::ofstream mFile;
	std::string mFilename;
};

#if defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)
class VisualStudioLogger : public Logger
{
public:
	VisualStudioLogger();
	virtual ~VisualStudioLogger();

	virtual void Write(const LogMessage& message);
};
#endif // defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)

} // namespace en

#define LogInfo(channel, importance, message, ...) //::en::LogManager::GetInstance().Write(en::LogType::Info, channel, message, __VA_ARGS__);
#define LogWarning(channel, importance, message, ...) //::en::LogManager::GetInstance().Write(en::LogType::Warning, channel, message, __VA_ARGS__);
#define LogError(channel, importance, message, ...) //::en::LogManager::GetInstance().Write(en::LogType::Error, channel, message, __VA_ARGS__);

#else

#define LogInfo(channel, importance, message, ...)
#define LogWarning(channel, importance, message, ...)
#define LogError(channel, importance, message, ...)

#endif // ENLIVE_ENABLE_LOG