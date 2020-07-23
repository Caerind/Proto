#pragma once

#include <imgui/imgui.h>

#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Vector4.hpp>
#include <Enlivengine/Math/Matrix3.hpp>
#include <Enlivengine/Math/Matrix4.hpp>
#include <Enlivengine/Math/Quaternion.hpp>
#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Core/Transform.hpp>
#include <Enlivengine/Graphics/Sprite.hpp>
#include <Enlivengine/Graphics/Text.hpp>

#include <Enlivengine/Core/CustomObjectEditor.hpp>
#include <Enlivengine/Core/ObjectEditor.hpp>

//#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Tools/ImGuiHelper.hpp>

// en::Time
template <>
struct CustomObjectEditor<en::Time>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Time& object, const char* name)
	{
		static constexpr std::size_t maxSize = 256;
		static char concatName[maxSize];
#ifdef ENLIVE_COMPILER_MSVC
		strcpy_s(concatName, name);
		strcat_s(concatName, " (s)");
#else
		strcpy(concatName, name);
		strcat(concatName, " (s)");
#endif // ENLIVE_COMPILER_MSVC
		float valueFloat = static_cast<float>(object.AsSeconds());
		if (ImGui::InputFloat(concatName, &valueFloat))
		{
			object = en::Time::Seconds(static_cast<en::F32>(valueFloat));
			return true;
		}
		return false;
	}
};

// en::Color
template <>
struct CustomObjectEditor<en::Color>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Color& object, const char* name)
	{
		static ImVec4 color;
		color = object.toImGuiColor();
		if (ImGui::ColorEdit4(name, (float*)&color, ImGuiColorEditFlags_AlphaPreview))
		{
			object.fromImGuiColor(color);
			return true;
		}
		return false;
	}
};

// en::Vector2
template <typename T>
struct CustomObjectEditor<en::Vector2<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Vector2<T>& object, const char* name)
	{
		if constexpr (en::Traits::IsIntegral<T>::value)
		{
			static int vec2i[2];
			vec2i[0] = object.x;
			vec2i[1] = object.y;
			if (ImGui::InputInt2(name, vec2i))
			{
				// TODO : NumericLimits<T> Max
				if constexpr (en::Traits::IsUnsigned<T>::value)
				{
					object.x = static_cast<T>((vec2i[0] >= 0) ? vec2i[0] : 0);
					object.y = static_cast<T>((vec2i[1] >= 0) ? vec2i[1] : 0);
				}
				else
				{
					object.x = static_cast<T>(vec2i[0]);
					object.y = static_cast<T>(vec2i[1]);
				}
				return true;
			}
			return false;
		}
		else if constexpr (en::Traits::IsFloatingPoint<T>::value)
		{
			static float vec2f[2];
			vec2f[0] = object.x;
			vec2f[1] = object.y;
			if (ImGui::InputFloat2(name, vec2f))
			{
				object.x = static_cast<T>(vec2f[0]);
				object.y = static_cast<T>(vec2f[1]);
				return true;
			}
			return false;
		}
		else
		{
			enLogWarning(en::LogChannel::System, "No custom editor for subtype {} for en::Vector2", en::TypeInfo<T>::GetName());
			return false;
		}
	}
};

// en::Vector3
template <typename T>
struct CustomObjectEditor<en::Vector3<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Vector3<T>& object, const char* name)
	{
		if constexpr (en::Traits::IsIntegral<T>::value)
		{
			static int vec3i[3];
			vec3i[0] = object.x;
			vec3i[1] = object.y;
			vec3i[2] = object.z;
			if (ImGui::InputInt3(name, vec3i))
			{
				// TODO : NumericLimits<T> Max
				if constexpr (en::Traits::IsUnsigned<T>::value)
				{
					object.x = static_cast<T>((vec3i[0] >= 0) ? vec3i[0] : 0);
					object.y = static_cast<T>((vec3i[1] >= 0) ? vec3i[1] : 0);
					object.z = static_cast<T>((vec3i[2] >= 0) ? vec3i[2] : 0);
				}
				else
				{
					object.x = static_cast<T>(vec3i[0]);
					object.y = static_cast<T>(vec3i[1]);
					object.z = static_cast<T>(vec3i[2]);
				}
				return true;
			}
			return false;
		}
		else if constexpr (en::Traits::IsFloatingPoint<T>::value)
		{
			static float vec3f[3];
			vec3f[0] = object.x;
			vec3f[1] = object.y;
			vec3f[2] = object.z;
			if (ImGui::InputFloat3(name, vec3f))
			{
				object.x = static_cast<T>(vec3f[0]);
				object.y = static_cast<T>(vec3f[1]);
				object.z = static_cast<T>(vec3f[2]);
				return true;
			}
			return false;
		}
		else
		{
			enLogWarning(en::LogChannel::System, "No custom editor for subtype {} for en::Vector3", en::TypeInfo<T>::GetName());
			return false;
		}
	}
};

// en::Vector4
template <typename T>
struct CustomObjectEditor<en::Vector4<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Vector4<T>& object, const char* name)
	{
		if constexpr (en::Traits::IsIntegral<T>::value)
		{
			static int vec4i[4];
			vec4i[0] = object.x;
			vec4i[1] = object.y;
			vec4i[2] = object.z;
			vec4i[3] = object.w;
			if (ImGui::InputInt4(name, vec4i))
			{
				// TODO : NumericLimits<T> Max
				if constexpr (en::Traits::IsUnsigned<T>::value)
				{
					object.x = static_cast<T>((vec4i[0] >= 0) ? vec4i[0] : 0);
					object.y = static_cast<T>((vec4i[1] >= 0) ? vec4i[1] : 0);
					object.z = static_cast<T>((vec4i[2] >= 0) ? vec4i[2] : 0);
					object.w = static_cast<T>((vec4i[3] >= 0) ? vec4i[3] : 0);
				}
				else
				{
					object.x = static_cast<T>(vec4i[0]);
					object.y = static_cast<T>(vec4i[1]);
					object.z = static_cast<T>(vec4i[2]);
					object.w = static_cast<T>(vec4i[3]);
				}
				return true;
			}
			return false;
		}
		else if constexpr (en::Traits::IsFloatingPoint<T>::value)
		{
			static float vec4f[4];
			vec4f[0] = object.x;
			vec4f[1] = object.y;
			vec4f[2] = object.z;
			vec4f[3] = object.w;
			if (ImGui::InputFloat4(name, vec4f))
			{
				object.x = static_cast<T>(vec3f[0]);
				object.y = static_cast<T>(vec3f[1]);
				object.z = static_cast<T>(vec3f[2]);
				object.w = static_cast<T>(vec4f[3]);
				return true;
			}
			return false;
		}
		else
		{
			enLogWarning(en::LogChannel::System, "No custom editor for subtype {} for en::Vector4", en::TypeInfo<T>::GetName());
			return false;
		}
	}
};

// en::Matrix3
template <typename T>
struct CustomObjectEditor<en::Matrix3<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Matrix3<T>& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			for (U32 row = 0; row < en::Matrix3<T>::rows; ++row)
			{
				static en::Vector3<T> matrixRow;
				matrixRow = object.getRow(row);
				if (CustomObjectEditor<en::Vector3<T>>::ImGuiEditor(matrixRow, ""))
				{
					object.setRow(row, matrixRow);
					modified = true;
				}
			}
		}
		return modified;
	}
};

// en::Matrix4
template <typename T>
struct CustomObjectEditor<en::Matrix4<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Matrix4<T>& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			for (U32 row = 0; row < en::Matrix4<T>::rows; ++row)
			{
				static en::Vector4<T> matrixRow;
				matrixRow = object.getRow(row);
				if (CustomObjectEditor<en::Vector4<T>>::ImGuiEditor(matrixRow, ""))
				{
					object.setRow(row, matrixRow);
					modified = true;
				}
			}
		}
		return modified;
	}
};

// en::Quaternion
template <typename T>
struct CustomObjectEditor<en::Quaternion<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Quaternion<T>& object, const char* name)
	{
		static en::Vector4<T> quat;
		quat = object.toVector4();
		if (CustomObjectEditor<en::Vector4<T>>::ImGuiEditor(quat, name))
		{
			object.fromVector4(quat);
			return true;
		}
		return false;
	}
};

// en::Rect
template <typename T>
struct CustomObjectEditor<en::Rect<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Rect<T>& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			en::Vector2<T> min = object.getMinimum();
			if (CustomObjectEditor<en::Vector2<T>>::ImGuiEditor(min, "Min"))
			{
				object.setMinimum(min);
				modified = true;
			}
			en::Vector2<T> size = object.getSize();
			if (CustomObjectEditor<en::Vector2<T>>::ImGuiEditor(size, "Size"))
			{
				object.setSize(size);
				modified = true;
			}
			ImGui::Unindent();
		}
		return modified;
	}
};

// en::Transform
template <>
struct CustomObjectEditor<en::Transform>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Transform& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();

			/*
			if (en::World* world = en::Universe::GetInstance().GetCurrentWorld())
			{
				world->GetFreeCamView().setCenter(object.GetPosition2D());
			}
			*/

			en::Vector3f position = object.GetPosition();
			if (en::ObjectEditor::ImGuiEditor(position, "Position"))
			{
				object.SetPosition(position);
				modified = true;
			}

			en::F32 rotation = object.GetRotation2D();
			if (en::ObjectEditor::ImGuiEditor(rotation, "Rotation"))
			{
				object.SetRotation2D(rotation);
				modified = true;
			}

			en::Vector3f scale = object.GetScale();
			if (en::ObjectEditor::ImGuiEditor(scale, "Scale"))
			{
				object.SetScale(scale);
				modified = true;
			}

			ImGui::Unindent();
		}
		return modified;
	}
};

// en::Sprite
template <>
struct CustomObjectEditor<en::Sprite>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Sprite& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();

			if (object.GetTexture() != nullptr)
			{
				const en::Recti iTexRect = object.GetTextureRect();
				en::Rectu uTexRect;
				// TODO : Ugly conv from recti to rectu...
				uTexRect.setMinimum(static_cast<en::U32>(iTexRect.getMinimum().x), static_cast<en::U32>(iTexRect.getMinimum().y));
				uTexRect.setSize(static_cast<en::U32>(iTexRect.getSize().x), static_cast<en::U32>(iTexRect.getSize().y));
				ImGui::PreviewTexture(*object.GetTexture(), uTexRect, 100.0f, false);
			}
			else
			{
				ImGui::TextColored(en::Color::Orange.toImGuiColor(), "Invalid texture");
			}

			static constexpr en::U32 resourceNameMaxSize = 255;
			static char resourceName[resourceNameMaxSize];
			if (ImGui::InputText("TextureIdentifier", resourceName, resourceNameMaxSize))
			{
				const std::string resourceNameStr = resourceName;
				const en::ResourceID resourceID = en::ResourceManager::StringToResourceID(resourceNameStr);
				auto& resourceManager = en::ResourceManager::GetInstance();
				if (resourceManager.Has(resourceID))
				{
					en::TexturePtr texturePtr = en::ResourceManager::GetInstance().Get<en::Texture>(resourceID);
					if (texturePtr.IsValid())
					{
						object.SetTexture(texturePtr.Get());
						modified = true;
					}
				}
			}

			en::Recti textureRect = object.GetTextureRect();
			if (en::ObjectEditor::ImGuiEditor(textureRect, "TextureRect"))
			{
				object.SetTextureRect(textureRect);
				modified = true;
			}

			en::Vector2f origin = object.GetOrigin();
			if (en::ObjectEditor::ImGuiEditor(origin, "Origin"))
			{
				object.SetOrigin(origin);
				modified = true;
			}

			ImGui::Unindent();
		}
		return modified;
	}
};

// en::Text
template <>
struct CustomObjectEditor<en::Text>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Text& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();

			static constexpr en::U32 resourceNameMaxSize = 255;
			static char resourceName[resourceNameMaxSize];
			if (ImGui::InputText("FontIdentifier", resourceName, resourceNameMaxSize))
			{
				const std::string resourceNameStr = resourceName;
				const en::ResourceID resourceID = en::ResourceManager::StringToResourceID(resourceNameStr);
				auto& resourceManager = en::ResourceManager::GetInstance();
				if (resourceManager.Has(resourceID))
				{
					en::FontPtr fontPtr = en::ResourceManager::GetInstance().Get<en::Font>(resourceID);
					if (fontPtr.IsValid())
					{
						object.SetFont(fontPtr.Get());
						modified = true;
					}
				}
			}

			std::string str = object.GetString();
			if (en::ObjectEditor::ImGuiEditor(str, "String"))
			{
				object.SetString(str);
				modified = true;
			}

			en::U32 characterSize = object.GetCharacterSize();
			if (en::ObjectEditor::ImGuiEditor(characterSize, "CharacterSize"))
			{
				object.SetCharacterSize(characterSize);
				modified = true;
			}

			en::F32 lineSpacing = object.GetLineSpacing();
			if (en::ObjectEditor::ImGuiEditor(lineSpacing, "LineSpacing"))
			{
				object.SetLineSpacing(lineSpacing);
				modified = true;
			}

			en::F32 letterSpacing = object.GetLetterSpacing();
			if (en::ObjectEditor::ImGuiEditor(letterSpacing, "LetterSpacing"))
			{
				object.SetLetterSpacing(letterSpacing);
				modified = true;
			}

			// TODO : Style

			en::Color fillColor = object.GetFillColor();
			if (en::ObjectEditor::ImGuiEditor(fillColor, "FillColor"))
			{
				object.SetFillColor(fillColor);
				modified = true;
			}

			en::Color outlineColor = object.GetOutlineColor();
			if (en::ObjectEditor::ImGuiEditor(outlineColor, "OutlineColor"))
			{
				object.SetOutlineColor(outlineColor);
				modified = true;
			}

			en::F32 outlineThickness = object.GetOutlineThickness();
			if (en::ObjectEditor::ImGuiEditor(outlineThickness, "OutlineThickness"))
			{
				object.SetOutlineThickness(outlineThickness);
				modified = true;
			}

			en::Vector2f origin = object.GetOrigin();
			if (en::ObjectEditor::ImGuiEditor(origin, "Origin"))
			{
				object.SetOrigin(origin);
				modified = true;
			}

			ImGui::Unindent();
		}
		return modified;
	}
};

// en::Entity
// TODO : Figure how to move it here

// en::EntityManager
// TODO : Figure how to move it here