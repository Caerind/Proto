#pragma once

#include "ObjectEditor.hpp"

#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/Math/Vector2.hpp>

// en::Time
template <>
struct CustomImGuiEditor<en::Time>
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
		float value = static_cast<float>(object.AsSeconds());
		if (ImGui::InputFloat(concatName, &value))
		{
			object = en::Time::Seconds(value);
			return true;
		}
		return false;
	}
};

// en::Color
template <>
struct CustomImGuiEditor<en::Color>
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

// en::Vector2f
template <>
struct CustomImGuiEditor<en::Vector2f>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Vector2f& object, const char* name)
	{
		static float vec2f[2];
		vec2f[0] = object.x;
		vec2f[1] = object.y;
		if (ImGui::InputFloat2(name, vec2f))
		{
			object.x = vec2f[0];
			object.y = vec2f[1];
			return true;
		}
		return false;
	}
};