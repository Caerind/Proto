#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/System/MetaEnum.hpp>
#include <Enlivengine/System/String.hpp>
#include <Enlivengine/System/TypeTraits.hpp>
#include <Enlivengine/System/ParserXml.hpp>

#include <array>
#include <vector>
#include <Enlivengine/System/Array.hpp>
#include <Enlivengine/System/Time.hpp>

template <typename T>
struct CustomSerialization
{
	// TODO : Maybe have a different value for serialization & deserialization
	static constexpr bool value = false;
};

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
	bool Serialize(const T* object, const char* name);

	template <typename T>
	bool Serialize_Registered(const T& object, const char* name);

	template <typename T>
	bool Deserialize(T& object, const char* name);
	template <typename T>
	bool Deserialize(T* object, const char* name);

	template <typename T>
	bool Deserialize_Registered(T& object, const char* name);

private:
	template <typename T>
	bool Serialize_Common(const T& object, const char* name);

	template <typename T>
	bool Serialize_Basic(const T& object, const char* name);
	template <typename T>
	bool Serialize_Basic(const en::Array<T>& object, const char* name);
	template <typename T>
	bool Serialize_Basic(const std::vector<T>& object, const char* name);
	template <typename T, std::size_t N>
	bool Serialize_Basic(const std::array<T, N>& object, const char* name);
	template <typename T>
	bool Serialize_Basic(const en::Array<T*>& object, const char* name);
	template <typename T>
	bool Serialize_Basic(const std::vector<T*>& object, const char* name);
	template <typename T, std::size_t N>
	bool Serialize_Basic(const std::array<T*, N>& object, const char* name);

	template <typename T>
	bool Deserialize_Common(T& object, const char* name);

	template <typename T>
	bool Deserialize_Basic(T& object, const char* name);
	template <typename T>
	bool Deserialize_Basic(en::Array<T>& object, const char* name);
	template <typename T>
	bool Deserialize_Basic(std::vector<T>& object, const char* name);
	template <typename T, std::size_t N>
	bool Deserialize_Basic(std::array<T, N>& object, const char* name);
	template <typename T>
	bool Deserialize_Basic(en::Array<T*>& object, const char* name);
	template <typename T>
	bool Deserialize_Basic(std::vector<T*>& object, const char* name);
	template <typename T, std::size_t N>
	bool Deserialize_Basic(std::array<T*, N>& object, const char* name);

private:
	en::ParserXml mParserXml;
	bool mValid;
};

template <typename T>
bool DataFile::Serialize(const T& object, const char* name)
{
	assert(mValid);
	assert(name != nullptr);

	return Serialize_Common(object, name);
}

template <typename T>
bool DataFile::Serialize(const T* object, const char* name)
{
	assert(mValid);
	assert(name != nullptr);
	assert(object != nullptr);

	return Serialize_Common(*object, name);
}

template <typename T>
bool DataFile::Serialize_Registered(const T& object, const char* name)
{
	static_assert(en::Meta::IsRegistered<T>());
	if (mParserXml.createChild(name))
	{
		mParserXml.setAttribute("classHash", en::Meta::GetClassVersion<T>());
		en::Meta::ForEachMember<T>([this, &object](const auto& member)
		{
			Serialize_Common(member.Get(object), member.GetName());
		});
		mParserXml.closeNode();
		return true;
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

	return Deserialize_Common(object, name);
}

template <typename T>
bool DataFile::Deserialize(T* object, const char* name)
{
	assert(mValid);
	assert(name != nullptr);
	assert(object != nullptr);

	return Deserialize_Common(*object, name);
}

template <typename T>
bool DataFile::Deserialize_Registered(T& object, const char* name)
{
	static_assert(en::Meta::IsRegistered<T>());
	if (mParserXml.readNode(name))
	{
		// TODO : Check classHash
		en::Meta::ForEachMember<T>([this, &object](const auto& member)
		{
			Deserialize_Common(member.Get(object), member.GetName());
		});
		mParserXml.closeNode();
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Serialize_Common(const T& object, const char* name)
{
	// TODO : Maybe have a different value for serialization & deserialization
	if constexpr (CustomSerialization<T>::value)
	{
		return CustomSerialization<T>::Serialize(*this, object, name);
	}
	else if constexpr (en::Meta::IsRegistered<T>())
	{
		return Serialize_Registered(object, name);
	}
	else
	{
		return Serialize_Basic(object, name);
	}
}

template <typename T>
bool DataFile::Serialize_Basic(const T& object, const char* name)
{
	assert(name);
	if (mParserXml.createChild(name))
	{
		if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, bool>::value)
		{
			mParserXml.setValue(en::toBoolString(object));
		}
		else if constexpr (en::Traits::IsEnum<T>::value)
		{
			const std::string enumValueStr(en::Meta::GetEnumName(object));
			mParserXml.setValue(enumValueStr);
		}
		else if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, std::string>::value)
		{
			mParserXml.setValue(object);
		}
		else if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, en::Time>::value)
		{
			mParserXml.setValue(en::toString(object.asSeconds()));
		}
		else
		{
			mParserXml.setValue(en::toString(object));
		}
		mParserXml.closeNode();
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Serialize_Basic(const en::Array<T>& object, const char* name)
{
	assert(name);
	if (mParserXml.createChild(name))
	{
		bool result = true;
		for (en::U32 i = 0; i < object.Size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			result = Serialize_Common(object[i], childName.c_str()) && result;
		}
		mParserXml.setAttribute("elementCount", object.Size());
		if constexpr (en::Meta::IsRegistered<T>())
		{
			mParserXml.setAttribute("elementClassHash", en::Meta::GetClassVersion<T>());
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
bool DataFile::Serialize_Basic(const std::vector<T>& object, const char* name)
{
	assert(name);
	if (mParserXml.createChild(name))
	{
		bool result = true;
		for (std::size_t i = 0; i < object.size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			result = Serialize_Common(object[i], childName.c_str()) && result;
		}
		mParserXml.setAttribute("elementCount", static_cast<en::U32>(object.size()));
		if constexpr (en::Meta::IsRegistered<T>())
		{
			mParserXml.setAttribute("elementClassHash", en::Meta::GetClassVersion<T>());
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
bool DataFile::Serialize_Basic(const std::array<T, N>& object, const char* name)
{
	assert(name);
	if (mParserXml.createChild(name))
	{
		bool result = true;
		for (std::size_t i = 0; i < object.size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			result = Serialize_Common(object[i], childName.c_str()) && result;
		}
		mParserXml.setAttribute("elementCount", static_cast<en::U32>(object.size()));
		if constexpr (en::Meta::IsRegistered<T>())
		{
			mParserXml.setAttribute("elementClassHash", en::Meta::GetClassVersion<T>());
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
bool DataFile::Serialize_Basic(const en::Array<T*>& object, const char* name)
{
	assert(name);
	if (mParserXml.createChild(name))
	{
		en::U32 elementCount = object.Size();
		bool result = true;
		for (en::U32 i = 0; i < object.Size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			if (object[i] != nullptr)
			{
				result = Serialize_Common(*object[i], childName.c_str()) && result;
			}
			else
			{
				elementCount--;
				// TODO : nullptr check
			}
		}
		mParserXml.setAttribute("elementCount", elementCount);
		if constexpr (en::Meta::IsRegistered<T>())
		{
			mParserXml.setAttribute("elementClassHash", en::Meta::GetClassVersion<T>());
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
bool DataFile::Serialize_Basic(const std::vector<T*>& object, const char* name)
{
	assert(name);
	if (mParserXml.createChild(name))
	{
		en::U32 elementCount = static_cast<en::U32>(object.size());
		bool result = true;
		for (std::size_t i = 0; i < object.size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			if (object[i] != nullptr)
			{
				result = Serialize_Common(*object[i], childName.c_str()) && result;
			}
			else
			{
				elementCount--;
				// TODO : nullptr check
			}
		}
		mParserXml.setAttribute("elementCount", elementCount);
		if constexpr (en::Meta::IsRegistered<T>())
		{
			mParserXml.setAttribute("elementClassHash", en::Meta::GetClassVersion<T>());
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
bool DataFile::Serialize_Basic(const std::array<T*, N>& object, const char* name)
{
	assert(name);
	if (mParserXml.createChild(name))
	{
		en::U32 elementCount = static_cast<en::U32>(object.size());
		bool result = true;
		for (std::size_t i = 0; i < object.size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			if (object[i] != nullptr)
			{
				result = Serialize_Common(*object[i], childName.c_str()) && result;
			}
			else
			{
				elementCount--;
				// TODO : nullptr check
			}
		}
		mParserXml.setAttribute("elementCount", elementCount);
		if constexpr (en::Meta::IsRegistered<T>())
		{
			mParserXml.setAttribute("elementClassHash", en::Meta::GetClassVersion<T>());
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
bool DataFile::Deserialize_Common(T& object, const char* name)
{
	// TODO : Maybe have a different value for serialization & deserialization
	if constexpr (CustomSerialization<T>::value)
	{
		return CustomSerialization<T>::Deserialize(*this, object, name);
	}
	else if constexpr (en::Meta::IsRegistered<T>())
	{
		return Deserialize_Registered(object, name);
	}
	else
	{
		return Deserialize_Basic(object, name);
	}
}

template <typename T>
bool DataFile::Deserialize_Basic(T& object, const char* name)
{
	assert(name);
	if (mParserXml.readNode(name))
	{
		if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, bool>::value)
		{
			std::string value;
			mParserXml.getValue(value);
			object = en::fromBoolString(value);
		}
		else if constexpr (en::Traits::IsEnum<T>::value)
		{
			std::string value;
			mParserXml.getValue(value);
			object = en::Meta::EnumCast<MyEnum>(value);
		}
		else if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, std::string>::value)
		{
			mParserXml.getValue(object);
		}
		else if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, en::Time>::value)
		{
			std::string value;
			mParserXml.getValue(value);
			object = en::seconds(en::fromString<en::F32>(value));
		}
		else
		{
			std::string value;
			mParserXml.getValue(value);
			object = en::fromString<T>(value);
		}
		mParserXml.closeNode();
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Deserialize_Basic(en::Array<T>& object, const char* name)
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
			result = Deserialize_Common(object[i], childName.c_str()) && result;
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
bool DataFile::Deserialize_Basic(std::vector<T>& object, const char* name)
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
			result = Deserialize_Common(object[i], childName.c_str()) && result;
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
bool DataFile::Deserialize_Basic(std::array<T, N>& object, const char* name)
{
	assert(name);
	if (mParserXml.readNode(name))
	{
		bool result = true;

		en::U32 elementCount = 0;
		mParserXml.getAttribute("elementCount", elementCount);
		assert(elementCount == static_cast<en::U32>(N));

		if constexpr (en::Meta::IsRegistered<T>())
		{
			// TODO : Test elementClassHash
		}

		for (en::U32 i = 0; i < elementCount; ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			result = Deserialize_Common(object[i], childName.c_str()) && result;
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
bool DataFile::Deserialize_Basic(en::Array<T*>& object, const char* name)
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
			if (object[i] != nullptr)
			{
				result = Deserialize_Common(*object[i], childName.c_str()) && result;
			}
			else
			{
				// TODO : nullptr check
			}
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
bool DataFile::Deserialize_Basic(std::vector<T*>& object, const char* name)
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
			if (object[i] != nullptr)
			{
				result = Deserialize_Common(*object[i], childName.c_str()) && result;
			}
			else
			{
				// TODO : nullptr check
			}
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
bool DataFile::Deserialize_Basic(std::array<T*, N>& object, const char* name)
{
	assert(name);
	if (mParserXml.readNode(name))
	{
		bool result = true;

		en::U32 elementCount = 0;
		mParserXml.getAttribute("elementCount", elementCount);
		assert(elementCount == static_cast<en::U32>(N));

		if constexpr (en::Meta::IsRegistered<T>())
		{
			// TODO : Test elementClassHash
		}

		for (en::U32 i = 0; i < elementCount; ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			if (object[i] != nullptr)
			{
				result = Deserialize_Common(*object[i], childName.c_str()) && result;
			}
			else
			{
				// TODO : nullptr check
			}
		}

		mParserXml.closeNode();
		return result;
	}
	else
	{
		return false;
	}
}