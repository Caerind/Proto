#pragma once

#include <entt/entt.hpp>

#include <Enlivengine/System/Meta.hpp>

#ifdef ENLIVE_ENABLE_IMGUI
#include <Enlivengine/Core/ObjectEditor.hpp>
#endif // ENLIVE_ENABLE_IMGUI
#include <Enlivengine/Core/DataFile.hpp>

namespace en
{

class ComponentManager
{
public:
	using ComponentTypeID = ENTT_ID_TYPE;

	ComponentManager() = delete;

	static bool IsRegistered(U32 componentHash)
	{
		return mComponents.find(componentHash) != mComponents.end();
	}

	template <typename T>
	static bool IsRegistered()
	{
		return IsRegistered(TypeInfo<T>::GetHash());
	}

	template <typename T>
	static bool Register()
	{
		static_assert(Meta::IsRegistered<T>());
		static_assert(TypeInfo<T>::IsKnown());
		constexpr U32 hash = TypeInfo<T>::GetHash();
		mComponents[hash].name = TypeInfo<T>::GetName();
		mComponents[hash].enttID = entt::type_info<T>::id();
#ifdef ENLIVE_ENABLE_IMGUI
		mComponents[hash].editor = [](entt::registry& reg, entt::entity ent)
		{
			return ObjectEditor::ImGuiEditor(reg.get<T>(ent), TypeInfo<T>::GetName());
		};
		mComponents[hash].add = [](entt::registry& reg, entt::entity ent)
		{
			reg.assign<T>(ent);
		};
		mComponents[hash].remove = [](entt::registry& reg, entt::entity ent)
		{
			reg.remove<T>(ent);
		};
#endif // ENLIVE_ENABLE_IMGUI
		mComponents[hash].serialize = [](DataFile& dataFile, const entt::registry& reg, entt::entity ent)
		{
			return dataFile.Serialize(reg.get<T>(ent), TypeInfo<T>::GetName());
		};
		mComponents[hash].deserialize = [](DataFile& dataFile, entt::registry& reg, entt::entity ent)
		{
			T& component = reg.assign<T>(ent);
			return dataFile.Deserialize(component, TypeInfo<T>::GetName());
		};
		return true;
	}

#ifdef ENLIVE_ENABLE_IMGUI
	using EditorCallback = std::function<bool(entt::registry&, entt::entity)>;
	using AddCallback = std::function<void(entt::registry&, entt::entity)>;
	using RemoveCallback = std::function<void(entt::registry&, entt::entity)>;
#endif // ENLIVE_ENABLE_IMGUI
	using SerializeCallback = std::function<bool(DataFile&, const entt::registry&, entt::entity)>;
	using DeserializeCallback = std::function<bool(DataFile&, entt::registry&, entt::entity)>;

	struct ComponentInfo
	{
		const char* name;
		ComponentTypeID enttID;
#ifdef ENLIVE_ENABLE_IMGUI
		EditorCallback editor;
		AddCallback add;
		RemoveCallback remove;
#endif // ENLIVE_ENABLE_IMGUI
		SerializeCallback serialize;
		DeserializeCallback deserialize;
	};

	static const std::unordered_map<U32, ComponentInfo>& GetComponentInfos() 
	{ 
		return mComponents; 
	}

private:
	static std::unordered_map<U32, ComponentInfo> mComponents;
};

} // namespace en