#include "DataFile.hpp"

#include <iostream>

DataFile::DataFile()
	: mParserXml()
	, mValid(false)
{
}

bool DataFile::IsValid() const
{
	return mValid;
}

bool DataFile::CreateEmptyFile()
{
	mParserXml.NewFile();
	if (mParserXml.CreateNode("DataFile"))
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
	if (mParserXml.LoadFromFile(filename))
	{
		if (mParserXml.ReadNode("DataFile"))
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
	return mParserXml.SaveToFile(filename);
}
