#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/System/String.hpp>
#include <Enlivengine/System/TypeTraits.hpp>
#include <Enlivengine/System/ParserXml.hpp>
#include <Enlivengine/System/Array.hpp>

#include <array>
#include <vector>

class DataFile
{
public:
	DataFile();

	bool IsValid() const;
	bool CreateEmptyFile();
	bool LoadFromFile(const std::string& filename);
	bool SaveToFile(const std::string& filename);

	template <typename T>
	bool Serialize(const T& object, const char* name);

	template <typename T>
	bool SerializeBasic(const T& object, const char* name);
	template <typename T>
	bool SerializeBasic(const en::Array<T>& object, const char* name);
	template <typename T>
	bool SerializeBasic(const std::vector<T>& object, const char* name);
	template <typename T, std::size_t N>
	bool SerializeBasic(const std::array<T, N>& object, const char* name);

	template <typename T>
	bool Deserialize(T& object, const char* name);

	template <typename T>
	bool DeserializeBasic(T& object, const char* name);
	template <typename T>
	bool DeserializeBasic(en::Array<T>& object, const char* name);
	template <typename T>
	bool DeserializeBasic(std::vector<T>& object, const char* name);
	template <typename T, std::size_t N>
	bool DeserializeBasic(std::array<T, N>& object, const char* name);

private:
	void OpenNode(const char* name);
	void CloseNode(const char* name);
	static std::string LevelToString(en::U32 level, en::U32 indentSize = 4); // TODO : Move to String.hpp ?

private:
	en::ParserXml mParserXml;
	bool mValid;

	en::U32 mLevel;
};

template <typename T>
bool DataFile::Serialize(const T& object, const char* name)
{
	assert(mValid);
	assert(name != nullptr);

	if constexpr (en::Meta::IsRegistered<T>())
	{
		if (mParserXml.createChild(name))
		{
			mParserXml.setAttribute("classHash", en::Meta::GetClassMembersHash<T>());
			en::Meta::ForAllMembers<T>(
				[this, &object](const auto& member)
				{
					if (member.canGetConstRef())
					{
						Serialize(member.get(object), member.getName());
					}
					else if (member.hasGetter())
					{
						auto copy = member.getCopy(object);
						Serialize(copy, member.getName());
					}
				}
			);
			mParserXml.closeNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return SerializeBasic(object, name);
	}
}

template <typename T>
bool DataFile::SerializeBasic(const T& object, const char* name)
{
	assert(name);
	if (mParserXml.createChild(name))
	{
		mParserXml.setValue(en::toString(object));
		mParserXml.closeNode();
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::SerializeBasic(const en::Array<T>& object, const char* name)
{
	assert(name);
	if (mParserXml.createChild(name))
	{
		mParserXml.setAttribute("elementCount", object.Size());

		if constexpr (en::Meta::IsRegistered<T>())
		{
			mParserXml.setAttribute("elementClassHash", en::Meta::GetClassMembersHash<T>());
		}

		bool result = true;
		for (en::U32 i = 0; i < object.Size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			result = Serialize(object[i], childName.c_str()) && result;
		}
		mParserXml.closeNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::SerializeBasic(const std::vector<T>& object, const char* name)
{
	assert(name);
	if (mParserXml.createChild(name))
	{
		mParserXml.setAttribute("elementCount", static_cast<en::U32>(object.size()));

		if constexpr (en::Meta::IsRegistered<T>())
		{
			mParserXml.setAttribute("elementClassHash", en::Meta::GetClassMembersHash<T>());
		}

		bool result = true;
		for (std::size_t i = 0; i < object.size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			result = Serialize(object[i], childName.c_str()) && result;
		}
		mParserXml.closeNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T, std::size_t N>
bool DataFile::SerializeBasic(const std::array<T, N>& object, const char* name)
{
	assert(name);
	if (mParserXml.createChild(name))
	{
		mParserXml.setAttribute("elementCount", static_cast<en::U32>(N));

		if constexpr (en::Meta::IsRegistered<T>())
		{
			mParserXml.setAttribute("elementClassHash", en::Meta::GetClassMembersHash<T>());
		}

		bool result = true;
		for (std::size_t i = 0; i < N; ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			result = Serialize(object[i], childName.c_str()) && result;
		}
		mParserXml.closeNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Deserialize(T& object, const char* name)
{
	assert(mValid);
	assert(name != nullptr);

	if constexpr (en::Meta::IsRegistered<T>())
	{
		if (mParserXml.readNode(name))
		{
			// TODO : Check classHash

			en::Meta::ForAllMembers<T>(
				[this, &object](const auto& member)
				{
					if (member.canGetRef())
					{
						Deserialize(member.getRef(object), member.getName());
					}
					else if (member.hasGetter())
					{
						using MemberT = meta::get_member_type<decltype(member)>;

						MemberT copy; // TODO : MemberT must be default constuctible
						Deserialize(copy, member.getName());
						member.set(object, copy);
					}
				}
			);

			mParserXml.closeNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return DeserializeBasic(object, name);
	}
}

template <typename T>
bool DataFile::DeserializeBasic(T& object, const char* name)
{
	assert(name);
	if (mParserXml.readNode(name))
	{
		std::string value;
		mParserXml.getValue(value);
		object = en::fromString<T>(value);

		mParserXml.closeNode();
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::DeserializeBasic(en::Array<T>& object, const char* name)
{
	assert(name);
	if (mParserXml.readNode(name))
	{
		bool result = true;

		en::U32 elementCount = 0;
		mParserXml.getAttribute("elementCount", elementCount);

		object.Resize(elementCount); // TODO : T must be default constructible

		if constexpr (en::Meta::IsRegistered<T>())
		{
			// TODO : Test elementClassHash
		}

		for (en::U32 i = 0; i < elementCount; ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));

			result = Deserialize(object[i], childName.c_str()) && result;
		}

		mParserXml.closeNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::DeserializeBasic(std::vector<T>& object, const char* name)
{
	assert(name);
	if (mParserXml.readNode(name))
	{
		bool result = true;

		en::U32 elementCount = 0;
		mParserXml.getAttribute("elementCount", elementCount);

		object.resize(elementCount); // TODO : T must be default constructible

		if constexpr (en::Meta::IsRegistered<T>())
		{
			// TODO : Test elementClassHash
		}

		for (en::U32 i = 0; i < elementCount; ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));

			result = Deserialize(object[i], childName.c_str()) && result;
		}

		mParserXml.closeNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T, std::size_t N>
bool DataFile::DeserializeBasic(std::array<T, N>& object, const char* name)
{
	assert(name);
	if (mParserXml.readNode(name))
	{
		bool result = true;

		en::U32 elementCount = 0;
		mParserXml.getAttribute("elementCount", elementCount);
		if (elementCount != static_cast<en::U32>(N))
		{
			result = false;
		}

		if constexpr (en::Meta::IsRegistered<T>())
		{
			// TODO : Test elementClassHash
		}

		for (en::U32 i = 0; i < elementCount; ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));

			result = Deserialize(object[i], childName.c_str()) && result;
		}

		mParserXml.closeNode();
		return result;
	}
	else
	{
		return false;
	}
}
