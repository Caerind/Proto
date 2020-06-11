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

// TODO : Factorise common code in Serialize/Deserialize(Array/vector/array)

template <typename T>
struct CustomSerialization
{
	// TODO : Maybe have a different value for serialization & deserialization
	static constexpr bool value = false;
};

class DataFile
{
public:
	DataFile(bool readable = true);

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
	template <typename T>
	void WriteCurrentType();
	en::U32 ReadCurrentType() const;

private:
	en::ParserXml mParserXml;
	bool mValid;
	bool mReadable;
};

template <typename T>
bool DataFile::Serialize(const T& object, const char* name)
{
	assert(mValid);
	assert(name != nullptr);
	if (mParserXml.HasNode(name))
	{
		mParserXml.RemoveNode(name);
	}
	return Serialize_Common(object, name);
}

template <typename T>
bool DataFile::Serialize(const T* object, const char* name)
{
	assert(object != nullptr);
	return Serialize(*object, name);
}

template <typename T>
bool DataFile::Serialize_Registered(const T& object, const char* name)
{
	static_assert(en::Meta::IsRegistered<T>());
	if (mParserXml.CreateNode(name))
	{
		WriteCurrentType<T>();
		en::Meta::ForEachMember<T>([this, &object](const auto& member)
		{
			Serialize_Common(member.Get(object), member.GetName());
		});
		mParserXml.CloseNode();
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
	assert(object != nullptr);
	return Deserialize(*object, name);
}

template <typename T>
bool DataFile::Deserialize_Registered(T& object, const char* name)
{
	static_assert(en::Meta::IsRegistered<T>());
	if (mParserXml.ReadNode(name))
	{
		const en::U32 typeHash = ReadCurrentType();
		if (typeHash == en::TypeInfo<T>::GetHash())
		{
			en::Meta::ForEachMember<T>([this, &object](const auto& member)
			{
				Deserialize_Common(member.Get(object), member.GetName());
			});
		}
		else
		{
			// TODO : Type mismatch ?
		}
		mParserXml.CloseNode();
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
	if (mParserXml.CreateNode(name))
	{
		WriteCurrentType<T>();
		if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, bool>::value)
		{
			mParserXml.SetValue(en::ToBoolString(object));
		}
		else if constexpr (en::Traits::IsEnum<T>::value)
		{
			const std::string enumValueStr(en::Meta::GetEnumName(object));
			mParserXml.SetValue(enumValueStr);
		}
		else if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, std::string>::value)
		{
			mParserXml.SetValue(object);
		}
		else if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, en::Time>::value)
		{
			mParserXml.SetValue(en::ToString(object.AsSeconds()));
		}
		else
		{
			mParserXml.SetValue(en::ToString(object));
		}
		mParserXml.CloseNode();
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
	if (mParserXml.CreateNode(name))
	{
		bool result = true;
		for (en::U32 i = 0; i < object.Size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			result = Serialize_Common(object[i], childName.c_str()) && result;
		}
		WriteCurrentType<T>();
		mParserXml.SetAttribute("size", object.Size());
		mParserXml.CloseNode();
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
	if (mParserXml.CreateNode(name))
	{
		bool result = true;
		for (std::size_t i = 0; i < object.size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			result = Serialize_Common(object[i], childName.c_str()) && result;
		}
		WriteCurrentType<T>();
		mParserXml.SetAttribute("size", static_cast<en::U32>(object.size()));
		mParserXml.CloseNode();
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
	if (mParserXml.CreateNode(name))
	{
		bool result = true;
		for (std::size_t i = 0; i < object.size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			result = Serialize_Common(object[i], childName.c_str()) && result;
		}
		WriteCurrentType<T>();
		mParserXml.SetAttribute("size", static_cast<en::U32>(object.size()));
		mParserXml.CloseNode();
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
	if (mParserXml.CreateNode(name))
	{
		en::U32 size = object.Size();
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
				size--;
				enLogWarning(en::LogChannel::Global, "Skipped nullptr object in {}", name);
			}
		}
		WriteCurrentType<T>();
		mParserXml.SetAttribute("size", size);
		mParserXml.CloseNode();
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
	if (mParserXml.CreateNode(name))
	{
		en::U32 size = static_cast<en::U32>(object.size());
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
				size--;
				enLogWarning(en::LogChannel::Global, "Skipped nullptr object in {}", name);
			}
		}
		WriteCurrentType<T>();
		mParserXml.SetAttribute("size", size);
		mParserXml.CloseNode();
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
	if (mParserXml.CreateNode(name))
	{
		en::U32 size = static_cast<en::U32>(object.size());
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
				size--;
				enLogWarning(en::LogChannel::Global, "Skipped nullptr object in {}", name);
			}
		}
		WriteCurrentType<T>();
		mParserXml.SetAttribute("size", size);
		mParserXml.CloseNode();
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
	if (mParserXml.ReadNode(name))
	{
		const en::U32 typeHash = ReadCurrentType();
		if (typeHash == en::TypeInfo<T>::GetHash())
		{
			if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, bool>::value)
			{
				std::string value;
				mParserXml.GetValue(value);
				object = en::FromBoolString(value);
			}
			else if constexpr (en::Traits::IsEnum<T>::value)
			{
				std::string value;
				mParserXml.GetValue(value);
				object = en::Meta::EnumCast<MyEnum>(value);
			}
			else if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, std::string>::value)
			{
				mParserXml.GetValue(object);
			}
			else if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, en::Time>::value)
			{
				std::string value;
				mParserXml.GetValue(value);
				object = en::Time::Seconds(en::FromString<en::F32>(value));
			}
			else
			{
				std::string value;
				mParserXml.GetValue(value);
				object = en::FromString<T>(value);
			}
		}
		else
		{
			// TODO : Type mismatch ?
		}
		mParserXml.CloseNode();
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
	if (mParserXml.ReadNode(name))
	{
		bool result = true;
		const en::U32 typeHash = ReadCurrentType();
		if (typeHash == en::TypeInfo<T>::GetHash())
		{
			en::U32 size = 0;
			mParserXml.GetAttribute("size", size);
			object.Resize(size);
			if constexpr (!en::Traits::IsDefaultConstructible<T>::value)
			{
				// For now T must be default constructible
				static_assert(en::Traits::IsDefaultConstructible<T>::value);
			}

			for (en::U32 i = 0; i < size; ++i)
			{
				std::string childName(name);
				childName.append("_");
				childName.append(std::to_string(i));
				result = Deserialize_Common(object[i], childName.c_str()) && result;
			}
		}
		else
		{
			// TODO : Type mismatch ?
		}
		mParserXml.CloseNode();
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
	if (mParserXml.ReadNode(name))
	{
		bool result = true;
		const en::U32 typeHash = ReadCurrentType();
		if (typeHash == en::TypeInfo<T>::GetHash())
		{
			en::U32 size = 0;
			mParserXml.GetAttribute("size", size);
			object.resize(static_cast<std::size_t>(size));
			if constexpr (!en::Traits::IsDefaultConstructible<T>:value)
			{
				// For now T must be default constructible
				static_assert(en::Traits::IsDefaultConstructible<T>::value);
			}

			for (en::U32 i = 0; i < size; ++i)
			{
				std::string childName(name);
				childName.append("_");
				childName.append(std::to_string(i));
				result = Deserialize_Common(object[i], childName.c_str()) && result;
			}
		}
		else
		{
			// TODO : Type mismatch ?
		}
		mParserXml.CloseNode();
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
	if (mParserXml.ReadNode(name))
	{
		bool result = true;
		const en::U32 typeHash = ReadCurrentType();
		if (typeHash == en::TypeInfo<T>::GetHash())
		{
			en::U32 size = 0;
			mParserXml.GetAttribute("size", size);
			if (size != static_cast<en::U32>(N))
			{
				// TODO : Find a solution to allow updates
				enLogError(en::LogChannel::Global, "Invalid size of {} for std::array named {} of size {}", size, name, N);
				return false;
			}
			if constexpr (!en::Traits::IsDefaultConstructible<T>::value)
			{
				// For now T must be default constructible
				static_assert(en::Traits::IsDefaultConstructible<T>::value);
			}

			for (en::U32 i = 0; i < size; ++i)
			{
				std::string childName(name);
				childName.append("_");
				childName.append(std::to_string(i));
				result = Deserialize_Common(object[i], childName.c_str()) && result;
			}
		}
		else
		{
			// TODO : Type mismatch ?
		}
		mParserXml.CloseNode();
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
	if (mParserXml.ReadNode(name))
	{
		bool result = true;
		const en::U32 typeHash = ReadCurrentType();
		if (typeHash == en::TypeInfo<T>::GetHash())
		{
			en::U32 size = 0;
			mParserXml.GetAttribute("size", size);
			object.Resize(size);

			for (en::U32 i = 0; i < size; ++i)
			{
				std::string childName(name);
				childName.append("_");
				childName.append(std::to_string(i));

				if (!mParserXml.ReadNode(childName))
				{
					enLogError(en::LogChannel::Global, "Can't read node {} for {}", childName, name);
					result = false;
					continue;
				}
				const en::U32 childTypeHash = ReadCurrentType();
				mParserXml.CloseNode();

				// TODO : Check inheritance childTypeHash is child of typeHash
				T* childObject = (T*)en::ClassManager::CreateClassFromHash(childTypeHash);
				if (childObject != nullptr)
				{
					const bool thisResult = Deserialize_Common(*childObject, childName.c_str());
					if (thisResult)
					{
						object[i] = childObject;
					}
					result = thisResult && result;
				}
				else
				{
					enLogError(en::LogChannel::Global, "Can't create object of type {} for {}", en::ClassManager::GetClassNameFromHash(childTypeHash), name);
					result = false;
				}
			}
		}
		else
		{
			// TODO : Type mismatch ?
		}

		mParserXml.CloseNode();
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
	if (mParserXml.ReadNode(name))
	{
		bool result = true;
		const en::U32 typeHash = ReadCurrentType();
		if (typeHash == en::TypeInfo<T>::GetHash())
		{
			en::U32 size = 0;
			mParserXml.GetAttribute("size", size);
			object.resize(static_cast<std::size_t>(size));

			for (en::U32 i = 0; i < size; ++i)
			{
				std::string childName(name);
				childName.append("_");
				childName.append(std::to_string(i));

				if (!mParserXml.ReadNode(childName))
				{
					enLogError(en::LogChannel::Global, "Can't read node {} for {}", childName, name);
					result = false;
					continue;
				}
				const en::U32 childTypeHash = ReadCurrentType();
				mParserXml.CloseNode();

				// TODO : Check inheritance childTypeHash is child of typeHash
				T* childObject = (T*)en::ClassManager::CreateClassFromHash(childTypeHash);
				if (childObject != nullptr)
				{
					const bool thisResult = Deserialize_Common(*childObject, childName.c_str());
					if (thisResult)
					{
						object[i] = childObject;
					}
					result = thisResult && result;
				}
				else
				{
					enLogError(en::LogChannel::Global, "Can't create object of type {} for {}", en::ClassManager::GetClassNameFromHash(childTypeHash), name);
					result = false;
				}
			}
		}
		else
		{
			// TODO : Type mismatch ?
		}

		mParserXml.CloseNode();
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
	if (mParserXml.ReadNode(name))
	{
		bool result = true;
		const en::U32 typeHash = ReadCurrentType();
		if (typeHash == en::TypeInfo<T>::GetHash())
		{
			en::U32 size = 0;
			mParserXml.GetAttribute("size", size);
			if (size != static_cast<en::U32>(N))
			{
				// TODO : Find a solution to allow updates
				enLogError(en::LogChannel::Global, "Invalid size of {} for std::array named {} of size {}", size, name, N);
				return false;
			}

			for (en::U32 i = 0; i < size; ++i)
			{
				std::string childName(name);
				childName.append("_");
				childName.append(std::to_string(i));

				if (!mParserXml.ReadNode(childName))
				{
					enLogError(en::LogChannel::Global, "Can't read node {} for {}", childName, name);
					result = false;
					continue;
				}
				const en::U32 childTypeHash = ReadCurrentType();
				mParserXml.CloseNode();

				// TODO : Check inheritance childTypeHash is child of typeHash
				T* childObject = (T*)en::ClassManager::CreateClassFromHash(childTypeHash);
				if (childObject != nullptr)
				{
					const bool thisResult = Deserialize_Common(*childObject, childName.c_str());
					if (thisResult)
					{
						object[i] = childObject;
					}
					result = thisResult && result;
				}
				else
				{
					enLogError(en::LogChannel::Global, "Can't create object of type {} for {}", en::ClassManager::GetClassNameFromHash(childTypeHash), name);
					result = false;
				}
			}
		}
		else
		{
			// TODO : Type mismatch ?
		}

		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T>
void DataFile::WriteCurrentType()
{
	if (mReadable)
	{
		mParserXml.SetAttribute("type", en::TypeInfo<T>::GetName());
	}
	else
	{
		mParserXml.SetAttribute("type", en::TypeInfo<T>::GetHash());
	}
}
