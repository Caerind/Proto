#include "DataFile.hpp"

#include <iostream>

DataFile::DataFile()
	: mParserXml()
	, mValid(false)
	, mLevel(0)
{
}

bool DataFile::IsValid() const
{
	return mValid;
}

bool DataFile::CreateEmptyFile()
{
	mParserXml.newFile();
	if (mParserXml.createChild("DataFile"))
	{
		mValid = true;
	}
	else
	{
		// TODO : LogError : Can't create "DataFile" root node for DataFile %s
		mValid = false;
	}
	return mValid;
}

bool DataFile::LoadFromFile(const std::string& filename)
{
	if (mParserXml.loadFromFile(filename))
	{
		if (mParserXml.readNode("DataFile"))
		{
			mValid = true;
		}
		else
		{
			mValid = false;
			// TODO : LogError : Root node "DataFile" is missing for DataFile %s
		}
	}
	else
	{
		mValid = false;
		// TODO : LogError : Invalid filename
	}
	return mValid;
}

bool DataFile::SaveToFile(const std::string& filename)
{
	return mParserXml.saveToFile(filename);
}

void DataFile::OpenNode(const char* name)
{
	std::cout << LevelToString(mLevel) << "<" << name << ">" << std::endl;
	mLevel++;
}

void DataFile::CloseNode(const char* name)
{
	mLevel--;
	std::cout << LevelToString(mLevel) << "</" << name << ">" << std::endl;
}

std::string DataFile::LevelToString(en::U32 level, en::U32 indentSize /*= 4*/)
{
	return std::string(static_cast<std::size_t>(level) * static_cast<std::size_t>(indentSize), ' ');
}
