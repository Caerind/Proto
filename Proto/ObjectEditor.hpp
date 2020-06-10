#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/System/MetaEnum.hpp>
#include <Enlivengine/System/String.hpp>
#include <Enlivengine/System/TypeTraits.hpp>
#include <Enlivengine/System/Array.hpp>

#include <array>
#include <vector>

#include <imgui/imgui.h>

template <typename T>
struct CustomImGuiEditor
{
	static constexpr bool value = false;
};

class ObjectEditor
{
public:
	template <typename T>
	static bool ImGuiEditorWindow(const char* windowName, T& object, const char* name)
	{
		assert(windowName != nullptr);
		assert(name != nullptr);
		bool modified = false;
		if (ImGui::Begin(windowName))
		{
			modified = ImGuiEditor_Common(object, name);
			ImGui::End();
		}
		return modified;
	}

	template <typename T>
	static bool ImGuiEditorWindow(const char* windowName, T* object, const char* name)
	{
		assert(windowName != nullptr);
		assert(object != nullptr);
		assert(name != nullptr);
		return ImGuiEditorWindow(windowName, *object, name);
	}

	template <typename T>
	static bool ImGuiEditor(T& object, const char* name)
	{
		assert(name != nullptr);
		return ImGuiEditor_Common(object, name);
	}

	template <typename T>
	static bool ImGuiEditor(T* object, const char* name)
	{
		assert(name != nullptr);
		assert(object != nullptr);
		return ImGuiEditor_Common(*object, name);
	}

	template <typename T>
	static bool ImGuiEditor_Registered(T& object, const char* name)
	{
		static_assert(en::Meta::IsRegistered<T>());
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			en::Meta::ForEachMember<T>([&object, &modified](const auto& member)
			{
				modified = ImGuiEditor_Common(member.Get(object), member.GetName()) || modified;
			});
			ImGui::Unindent();
		}
		return modified;
	}

private:
	template <typename T>
	static bool ImGuiEditor_Common(T& object, const char* name)
	{
		if constexpr (CustomImGuiEditor<T>::value)
		{
			return CustomImGuiEditor<T>::ImGuiEditor(object, name);
		}
		else if constexpr (en::Meta::IsRegistered<T>())
		{
			return ImGuiEditor_Registered(object, name);
		}
		else
		{
			return ImGuiEditor_Basic(object, name);
		}
	}

	template <typename T>
	static bool ImGuiEditor_Basic(T& object, const char* name)
	{
		assert(name != nullptr);

		if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, bool>::value)
		{
			return ImGui::Checkbox(name, &object);
		}
		else if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, char>::value)
		{
			static char c;
			c = object;
			if (ImGui::InputText(name, &c, 2))
			{
				object = c;
				return true;
			}
			return false;
		}
		else if constexpr (en::Traits::IsEnum<T>::value)
		{
			static bool initializedEnum = false;
			static constexpr en::U32 enumCount = en::Meta::GetEnumCount<T>();
			static std::string stringsStorage[enumCount];
			static const char* stringsImGui[enumCount];
			if (!initializedEnum)
			{
				auto enumNames = en::Meta::GetEnumNames<T>();
				for (en::U32 i = 0; i < enumCount; ++i)
				{
					stringsStorage[i] = enumNames[i];
					stringsImGui[i] = stringsStorage[i].c_str();
				}
				initializedEnum = true;
			}
			int index = static_cast<int>(en::Meta::GetEnumIndex(object));
			if (ImGui::Combo(name, &index, stringsImGui, IM_ARRAYSIZE(stringsImGui)))
			{
				object = en::Meta::GetEnumFromIndex<T>(static_cast<en::U32>(index));
				return true;
			}
			return false;
		}
		else if constexpr (en::Traits::IsIntegral<T>::value)
		{
			int value = static_cast<int>(object);
			if (ImGui::InputInt(name, &value))
			{
				// TODO : NumericLimits<T> Max
				if constexpr (en::Traits::IsUnsigned<T>::value)
				{
					object = static_cast<T>((value >= 0) ? value : 0);
				}
				else
				{
					object = static_cast<T>(value);
				}
				return true;
			}
			return false;
		}
		else if constexpr (en::Traits::IsFloatingPoint<T>::value)
		{
			float value = static_cast<float>(object);
			if (ImGui::InputFloat(name, &value))
			{
				object = static_cast<T>(value);
				return true;
			}
			return false;
		}
		else if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, std::string>::value)
		{
			static constexpr std::size_t maxSize = 256;
			static char inputBuffer[maxSize];
			if (strcmp(inputBuffer, object.c_str()) != 0)
			{
#ifdef ENLIVE_COMPILER_MSVC
				strcpy_s(inputBuffer, object.c_str());
#else
				strcpy(inputBuffer, object.c_str());
#endif // ENLIVE_COMPILER_MSVC
			}
			if (ImGui::InputText(name, inputBuffer, maxSize))
			{
				object = inputBuffer;
				return true;
			}
			return false;
		}
		else if constexpr (en::Traits::IsSame<en::Traits::Decay<T>::type, en::Time>::value)
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
		else
		{
			ImGui::Text("%s type is not implemented for imgui for %s", en::TypeInfo<T>::GetName(), name);
			return false;
		}
	}

	template <typename T>
	static bool ImGuiEditor_Basic(std::vector<T>& object, const char* name)
	{
		assert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (std::size_t i = 0; i < object.size(); ++i)
			{
				std::string childName(name);
				childName.append("[");
				childName.append(std::to_string(i));
				childName.append("]");
				modified = ImGuiEditor_Common(object[i], childName.c_str()) || modified;
			}
			ImGui::Unindent();
		}
		return modified;
	}

	template <typename T>
	static bool ImGuiEditor_Basic(en::Array<T>& object, const char* name)
	{
		assert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (en::U32 i = 0; i < object.Size(); ++i)
			{
				std::string childName(name);
				childName.append("[");
				childName.append(std::to_string(i));
				childName.append("]");
				modified = ImGuiEditor_Common(object[i], childName.c_str()) || modified;
			}
			ImGui::Unindent();
		}
		return modified;
	}

	template <typename T, std::size_t N>
	static bool ImGuiEditor_Basic(std::array<T, N>& object, const char* name)
	{
		assert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (std::size_t i = 0; i < object.size(); ++i)
			{
				std::string childName(name);
				childName.append("[");
				childName.append(std::to_string(i));
				childName.append("]");
				modified = ImGuiEditor_Common(object[i], childName.c_str()) || modified;
			}
			ImGui::Unindent();
		}
		return modified;
	}

	template <typename T>
	static bool ImGuiEditor_Basic(std::vector<T*>& object, const char* name)
	{
		assert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (std::size_t i = 0; i < object.size(); ++i)
			{
				std::string childName(name);
				childName.append("[");
				childName.append(std::to_string(i));
				childName.append("]");
				if (object[i] != nullptr)
				{
					modified = ImGuiEditor_Common(*object[i], childName.c_str()) || modified;
				}
				else
				{
					// TODO : nullptr check
				}
			}
			ImGui::Unindent();
		}
		return modified;
	}

	template <typename T>
	static bool ImGuiEditor_Basic(en::Array<T*>& object, const char* name)
	{
		assert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (en::U32 i = 0; i < object.Size(); ++i)
			{
				std::string childName(name);
				childName.append("[");
				childName.append(std::to_string(i));
				childName.append("]");
				if (object[i] != nullptr)
				{
					modified = ImGuiEditor_Common(*object[i], childName.c_str()) || modified;
				}
				else
				{
					// TODO : nullptr check
				}
			}
			ImGui::Unindent();
		}
		return modified;
	}

	template <typename T, std::size_t N>
	static bool ImGuiEditor_Basic(std::array<T*, N>& object, const char* name)
	{
		assert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			for (std::size_t i = 0; i < object.size(); ++i)
			{
				std::string childName(name);
				childName.append("[");
				childName.append(std::to_string(i));
				childName.append("]");
				if (object[i] != nullptr)
				{
					modified = ImGuiEditor_Common(*object[i], childName.c_str()) || modified;
				}
				else
				{
					// TODO : nullptr check
				}
			}
			ImGui::Unindent();
		}
		return modified;
	}
};
