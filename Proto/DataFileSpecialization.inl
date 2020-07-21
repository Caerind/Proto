#pragma once

#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Vector4.hpp>
#include <Enlivengine/Math/Matrix3.hpp>
#include <Enlivengine/Math/Matrix4.hpp>
#include <Enlivengine/Math/Quaternion.hpp>

#include "CustomXmlSerialization.hpp"
#include "DataFile.hpp"

// en::Time
template <>
struct CustomXmlSerialization<en::Time>
{
	static constexpr bool value = true;
	static bool Serialize(DataFile& dataFile, const en::Time& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			parser.SetValue(en::ToString(object.AsSeconds()));
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(DataFile& dataFile, en::Time& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Time>::GetHash());
			std::string value;
			parser.GetValue(value);
			object = en::Time::Seconds(en::FromString<en::F32>(value));
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

// en::Color
template <>
struct CustomXmlSerialization<en::Color>
{
	static constexpr bool value = true;
	static bool Serialize(DataFile& dataFile, const en::Color& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			parser.SetValue(object.toString());
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(DataFile& dataFile, en::Color& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Color>::GetHash());
			std::string value;
			parser.GetValue(value);
			object.fromString(value);
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

// en::Vector2
template <typename T>
struct CustomXmlSerialization<en::Vector2<T>>
{
	static constexpr bool value = true;
	static bool Serialize(DataFile& dataFile, const en::Vector2<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Vector2<T>>();
			dataFile.Serialize_Basic(object.x, "x");
			dataFile.Serialize_Basic(object.y, "y");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(DataFile& dataFile, en::Vector2<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Vector2<T>>::GetHash());
			dataFile.Deserialize_Basic(object.x, "x");
			dataFile.Deserialize_Basic(object.y, "y");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

// en::Vector3
template <typename T>
struct CustomXmlSerialization<en::Vector3<T>>
{
	static constexpr bool value = true;
	static bool Serialize(DataFile& dataFile, const en::Vector3<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Vector3<T>>();
			dataFile.Serialize_Basic(object.x, "x");
			dataFile.Serialize_Basic(object.y, "y");
			dataFile.Serialize_Basic(object.z, "z");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(DataFile& dataFile, en::Vector3<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Vector3<T>>::GetHash());
			dataFile.Deserialize_Basic(object.x, "x");
			dataFile.Deserialize_Basic(object.y, "y");
			dataFile.Deserialize_Basic(object.z, "z");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

// en::Vector4
template <typename T>
struct CustomXmlSerialization<en::Vector4<T>>
{
	static constexpr bool value = true;
	static bool Serialize(DataFile& dataFile, const en::Vector4<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Vector4<T>>();
			dataFile.Serialize_Basic(object.x, "x");
			dataFile.Serialize_Basic(object.y, "y");
			dataFile.Serialize_Basic(object.z, "z");
			dataFile.Serialize_Basic(object.w, "w");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(DataFile& dataFile, en::Vector4<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Vector4<T>>::GetHash());
			dataFile.Deserialize_Basic(object.x, "x");
			dataFile.Deserialize_Basic(object.y, "y");
			dataFile.Deserialize_Basic(object.z, "z");
			dataFile.Deserialize_Basic(object.w, "w");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

// en::Matrix3
template <typename T>
struct CustomXmlSerialization<en::Matrix3<T>>
{
	static constexpr bool value = true;
	static bool Serialize(DataFile& dataFile, const en::Matrix3<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Matrix3<T>>();
			for (std::size_t i = 0; i < en::Matrix3<T>::elements; ++i)
			{
				std::string childName(std::to_string(i));
				dataFile.Serialize_Common(object[i], childName.c_str());
			}
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(DataFile& dataFile, en::Matrix3<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Matrix3<T>>::GetHash());
			for (std::size_t i = 0; i < en::Matrix3<T>::elements; ++i)
			{
				std::string childName(std::to_string(i));
				dataFile.Deserialize_Common(object[i], childName.c_str());
			}
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

// en::Matrix4
template <typename T>
struct CustomXmlSerialization<en::Matrix4<T>>
{
	static constexpr bool value = true;
	static bool Serialize(DataFile& dataFile, const en::Matrix4<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Matrix4<T>>();
			for (std::size_t i = 0; i < en::Matrix4<T>::elements; ++i)
			{
				std::string childName(std::to_string(i));
				dataFile.Serialize_Common(object[i], childName.c_str());
			}
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(DataFile& dataFile, en::Matrix4<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Matrix4<T>>::GetHash());
			for (std::size_t i = 0; i < en::Matrix4<T>::elements; ++i)
			{
				std::string childName(std::to_string(i));
				dataFile.Deserialize_Common(object[i], childName.c_str());
			}
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

// en::Quaternion
template <typename T>
struct CustomXmlSerialization<en::Quaternion<T>>
{
	static constexpr bool value = true;
	static bool Serialize(DataFile& dataFile, const en::Quaternion<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Quaternion<T>>();
			dataFile.Serialize_Common(v.x, "x");
			dataFile.Serialize_Common(v.y, "y");
			dataFile.Serialize_Common(v.z, "z");
			dataFile.Serialize_Common(s, "s");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(DataFile& dataFile, en::Quaternion<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Quaternion<T>>::GetHash());
			dataFile.Deserialize_Common(v.x, "x");
			dataFile.Deserialize_Common(v.y, "y");
			dataFile.Deserialize_Common(v.z, "z");
			dataFile.Deserialize_Common(s, "s");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

// en::Entity
// TODO : Figure how to move it here

// en::EntityManager
// TODO : Figure how to move it here