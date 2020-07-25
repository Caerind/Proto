#pragma once

#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Vector4.hpp>
#include <Enlivengine/Math/Matrix3.hpp>
#include <Enlivengine/Math/Matrix4.hpp>
#include <Enlivengine/Math/Quaternion.hpp>
#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Graphics/Sprite.hpp>
#include <Enlivengine/Graphics/Text.hpp>

#include <Enlivengine/Core/CustomXmlSerialization.hpp>
#include <Enlivengine/Core/DataFile.hpp>

// en::Time
template <>
struct CustomXmlSerialization<en::Time>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Time& object, const char* name)
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
	static bool Deserialize(en::DataFile& dataFile, en::Time& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Time>::GetHash());
			std::string valueString;
			parser.GetValue(valueString);
			object = en::Time::Seconds(en::FromString<en::F32>(valueString));
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
	static bool Serialize(en::DataFile& dataFile, const en::Color& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Color>();
			parser.SetValue(object.toString());
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Color& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Color>::GetHash());
			std::string valueString;
			parser.GetValue(valueString);
			object.fromString(valueString);
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
	static bool Serialize(en::DataFile& dataFile, const en::Vector2<T>& object, const char* name)
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
	static bool Deserialize(en::DataFile& dataFile, en::Vector2<T>& object, const char* name)
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
	static bool Serialize(en::DataFile& dataFile, const en::Vector3<T>& object, const char* name)
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
	static bool Deserialize(en::DataFile& dataFile, en::Vector3<T>& object, const char* name)
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
	static bool Serialize(en::DataFile& dataFile, const en::Vector4<T>& object, const char* name)
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
	static bool Deserialize(en::DataFile& dataFile, en::Vector4<T>& object, const char* name)
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
	static bool Serialize(en::DataFile& dataFile, const en::Matrix3<T>& object, const char* name)
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
	static bool Deserialize(en::DataFile& dataFile, en::Matrix3<T>& object, const char* name)
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
	static bool Serialize(en::DataFile& dataFile, const en::Matrix4<T>& object, const char* name)
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
	static bool Deserialize(en::DataFile& dataFile, en::Matrix4<T>& object, const char* name)
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
	static bool Serialize(en::DataFile& dataFile, const en::Quaternion<T>& object, const char* name)
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
	static bool Deserialize(en::DataFile& dataFile, en::Quaternion<T>& object, const char* name)
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

// en::Rect
template <typename T>
struct CustomXmlSerialization<en::Rect<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Rect<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Rect<T>>();
			dataFile.Serialize_Common(object.getMinimum(), "Min");
			dataFile.Serialize_Common(object.getSize(), "Size");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Rect<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Rect<T>>::GetHash());
			en::Vector2<T> min;
			dataFile.Deserialize_Common(min, "Min");
			object.setMinimum(min);
			en::Vector2<T> size;
			dataFile.Deserialize_Common(size, "Size");
			object.setSize(size);
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

// en::Sprite
template <>
struct CustomXmlSerialization<en::Sprite>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Sprite& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Sprite>();
			dataFile.Serialize_Common(static_cast<en::U32>(object.GetTextureID()), "TextureID");
			dataFile.Serialize_Common(object.GetTextureRect(), "TextureRect");
			dataFile.Serialize_Common(object.GetOrigin(), "Origin");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Sprite& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Sprite>::GetHash());

			en::U32 textureID;
			dataFile.Deserialize_Common(textureID, "TextureID");
			const en::ResourceID textureResourceID = static_cast<en::ResourceID>(textureID);
			if (en::ResourceManager::GetInstance().Has(textureResourceID))
			{
				en::TexturePtr texturePtr = en::ResourceManager::GetInstance().Get<en::Texture>(textureResourceID);
				if (texturePtr.IsValid())
				{
					object.SetTexture(texturePtr.Get());
				}
			}

			en::Recti textureRect;
			dataFile.Deserialize_Common(textureRect, "TextureRect");
			object.SetTextureRect(textureRect);

			en::Vector2f origin;
			dataFile.Deserialize_Common(origin, "Origin");
			object.SetOrigin(origin);

			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

// en::Text
template <>
struct CustomXmlSerialization<en::Text>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Text& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Text>();
			dataFile.Serialize_Common(static_cast<en::U32>(object.GetFontID()), "FontID");
			std::string str = object.GetString();
			dataFile.Serialize_Common(str, "String");
			dataFile.Serialize_Common(object.GetCharacterSize(), "CharacterSize");
			dataFile.Serialize_Common(object.GetLineSpacing(), "LineSpacing");
			dataFile.Serialize_Common(object.GetLetterSpacing(), "LetterSpacing");
			// TODO : Style
			dataFile.Serialize_Common(object.GetFillColor(), "FillColor");
			dataFile.Serialize_Common(object.GetOutlineColor(), "OutlineColor");
			dataFile.Serialize_Common(object.GetOutlineThickness(), "OutlineThickness");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Text& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Text>::GetHash());

			en::U32 fontID;
			dataFile.Deserialize_Common(fontID, "FontID");
			const en::ResourceID fontResourceID = static_cast<en::ResourceID>(fontID);
			if (en::ResourceManager::GetInstance().Has(fontResourceID))
			{
				en::FontPtr fontPtr = en::ResourceManager::GetInstance().Get<en::Font>(fontResourceID);
				if (fontPtr.IsValid())
				{
					object.SetFont(fontPtr.Get());
				}
			}

			std::string str;
			dataFile.Deserialize_Common(str, "String");
			object.SetString(str);

			en::U32 characterSize;
			dataFile.Deserialize_Common(characterSize, "CharacterSize");
			object.SetCharacterSize(characterSize);

			en::F32 lineSpacing;
			dataFile.Deserialize_Common(lineSpacing, "LineSpacing");
			object.SetLineSpacing(lineSpacing);

			en::F32 letterSpacing;
			dataFile.Deserialize_Common(letterSpacing, "LetterSpacing");
			object.SetLetterSpacing(letterSpacing);

			// TODO : Style

			en::Color fillColor;
			dataFile.Deserialize_Common(fillColor, "FillColor");
			object.SetFillColor(fillColor);

			en::Color outlineColor;
			dataFile.Deserialize_Common(outlineColor, "OutlineColor");
			object.SetOutlineColor(outlineColor);

			en::F32 outlineThickness;
			dataFile.Deserialize_Common(outlineThickness, "OutlineThickness");
			object.SetOutlineThickness(outlineThickness);

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