#include "DataFile.hpp"

DataFile::DataFile(bool readable)
	: mParserXml()
	, mValid(false)
	, mReadable(readable)
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
		enLogError(en::LogChannel::Global, "Can't create \"DataFile\" root node for new DataFile");
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
			enLogError(en::LogChannel::Global, "Can't find \"DataFile\" root node for DataFile {}", filename);
			mValid = false;
		}
	}
	else
	{
		enLogError(en::LogChannel::Global, "Invalid DataFile file {}", filename);
		mValid = false;
	}
	return mValid;
}

bool DataFile::SaveToFile(const std::string& filename)
{
	return mParserXml.SaveToFile(filename);
}

en::U32 DataFile::ReadCurrentType() const
{
	// TODO : Improve perfs of this method based on mReadable for non debug versions
	std::string typeStr;
	mParserXml.GetAttribute("type", typeStr);
	if (en::IsNumber(typeStr))
	{
		return en::FromString<en::U32>(typeStr);
	}
	else
	{
		return en::Hash::ConstexprHash(typeStr.c_str());
	}
}
