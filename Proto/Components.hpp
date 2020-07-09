#pragma once

#include <entt/entt.hpp>

#include <string>

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/TypeInfo.hpp>
#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/Config.hpp>

#include <Enlivengine/Math/Vector2.hpp>

#include "ObjectEditor.hpp"

namespace en
{

class EntityManager
{
public:
	entt::registry mRegistry;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::EntityManager)
ENLIVE_META_CLASS_END()



namespace en
{

	class Component
	{
	public:
		static const char* GetStaticClassName();
		static U32 GetStaticClassHash();
		virtual const char* GetClassName() { return GetStaticClassName(); }
		virtual U32 GetClassHash() const { return GetStaticClassHash(); }
	};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Component)
ENLIVE_META_CLASS_END()




namespace en
{

class ComponentManager
{
public:
	using ComponentTypeID = ENTT_ID_TYPE;

	ComponentManager() = delete;

	template <typename T>
	static bool Register()
	{
		static_assert(Meta::IsRegistered<T>());
		static_assert(TypeInfo<T>::IsKnown());
		static_assert(Traits::IsBaseOf<Component, T>::value);
		constexpr U32 hash = TypeInfo<T>::GetHash();
		mComponents[hash].name = TypeInfo<T>::GetName();
		mComponents[hash].enttID = entt::type_info<T>::id();
		mComponents[hash].editor = [](entt::registry& reg, entt::entity& ent)
		{
			return ObjectEditor::ImGuiEditor_Registered(reg.get<T>(ent), TypeInfo<T>::GetName());
		};
		return true;
	}

	static bool HasComponent(entt::registry& reg, entt::entity& ent, U32 enttComponentID)
	{
		ComponentTypeID type[] = { enttComponentID };
		return reg.runtime_view(std::cbegin(type), std::cend(type)).contains(ent);
	}
	
	using EditorCallback = std::function<bool(entt::registry& reg, entt::entity& ent)>;

	struct ComponentInfo
	{
		const char* name;
		ComponentTypeID enttID;
		EditorCallback editor;
	};
	static std::unordered_map<U32, ComponentInfo> mComponents;
};

} // namespace en


namespace en
{

class Entity
{
public:
	EntityManager* mManager;
	entt::entity mEntity;
};

template <>
struct CustomImGuiEditor<Entity>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(Entity& object, const char* name)
	{
		if (ImGui::CollapsingHeader(name))
		{
			bool result = false;
			ImGui::Indent();
			if (object.mManager != nullptr && object.mManager->mRegistry.valid(object.mEntity))
			{
				const auto endItr = ComponentManager::mComponents.end();
				for (auto itr = ComponentManager::mComponents.begin(); itr != endItr; ++itr)
				{
					if (ComponentManager::HasComponent(object.mManager->mRegistry, object.mEntity, itr->second.enttID))
					{
						if (itr->second.editor(object.mManager->mRegistry, object.mEntity))
						{
							return true;
						}
					}
				}
			}
			ImGui::Unindent();
		}
		return true;
	}
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Entity)
ENLIVE_META_CLASS_END()






#ifdef ENLIVE_DEBUG

namespace en
{

struct NameComponent : public Component
{
	static const char* GetStaticClassName();
	static U32 GetStaticClassHash();
	virtual const char* GetClassName() { return GetStaticClassName(); }
	virtual U32 GetClassHash() const { return GetStaticClassHash(); }

	std::string name;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::NameComponent)
	ENLIVE_META_CLASS_MEMBER("name", &en::NameComponent::name)
ENLIVE_META_CLASS_END()

#endif // ENLIVE_DEBUG






namespace en
{

struct PositionComponent : public Component
{
	static const char* GetStaticClassName();
	static U32 GetStaticClassHash();
	virtual const char* GetClassName() { return GetStaticClassName(); }
	virtual U32 GetClassHash() const { return GetStaticClassHash(); }

	en::Vector2f position;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::PositionComponent)
	ENLIVE_META_CLASS_MEMBER("position", &en::PositionComponent::position)
ENLIVE_META_CLASS_END()

