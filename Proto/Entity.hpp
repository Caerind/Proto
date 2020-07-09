#pragma once

#include <entt/entt.hpp>

#include <Enlivengine/System/Meta.hpp>

#include "ComponentManager.hpp"

namespace en
{

class EntityManager;

// This is only a handle of an entity in a registry
// It does not create or destroy entity from registry
// It is used for CustomEditor/Serialization and convenience
class Entity
{
public:
	Entity();
	Entity(EntityManager& manager, entt::entity entity);

	bool IsValid() const;
	U32 GetID() const;

#ifdef ENLIVE_DEBUG
	const char* GetName() const;
#endif // ENLIVE_DEBUG

	template <typename T> 
	T& Add();

	template <typename T, typename ...Args> 
	T& Add(Args&&  ...args);

	template <typename ...Components> 
	bool Has() const;

	template <typename T> 
	void Remove();

	template <typename T> 
	T& Get();

	template <typename T> 
	const T& Get() const;

	bool HasManager() const;
	EntityManager& GetManager();
	const EntityManager& GetManager() const;

	// Using these should be avoided if possible
	const entt::entity& GetEntity() const;
	entt::registry& GetRegistry();
	const entt::registry& GetRegistry() const;

private:
	friend class EntityManager;

private:
	EntityManager* mManager;
	entt::entity mEntity;
};

template <typename T> 
T& Entity::Add()
{
	assert(ComponentManager::IsRegistered<T>());
	assert(IsValid());

	return GetRegistry().assign<T>(mEntity); 
}

template <typename T, typename ...Args>
T& Entity::Add(Args&& ...args)
{
	assert(ComponentManager::IsRegistered<T>());
	assert(IsValid()); 
	
	return GetRegistry().emplace<T>(mEntity, std::forward<Args>(args)...); 
}

template <typename ...Components> 
bool Entity::Has() const 
{ 
	assert(IsValid()); 

	return GetRegistry().has<Components...>(mEntity); 
}

template <typename T> 
void Entity::Remove()
{
	assert(ComponentManager::IsRegistered<T>());
	assert(IsValid());
	assert(Has<T>());
	
	GetRegistry().remove<T>(mEntity); 
}

template <typename T> 
T& Entity::Get()
{
	assert(ComponentManager::IsRegistered<T>());
	assert(IsValid());
	assert(Has<T>());

	return GetRegistry().get<T>(mEntity); 
}

template <typename T> 
const T& Entity::Get() const 
{
	assert(ComponentManager::IsRegistered<T>());
	assert(IsValid());
	assert(Has<T>());

	return GetRegistry().get<T>(mEntity); 
}

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Entity)
ENLIVE_META_CLASS_END()

// TODO : Figure out how to move it elsewhere
template <>
struct CustomImGuiEditor<en::Entity>
{
	using ComponentTypeID = ENTT_ID_TYPE;

	static constexpr bool value = true;
	static bool ImGuiEditor(en::Entity& object, const char* name)
	{
		if (ImGui::CollapsingHeader(name))
		{
			bool result = false;
			ImGui::Indent();
			if (object.IsValid())
			{
				const en::U32 entityID = object.GetID();
				ImGui::Text("ID: %d", entityID);
				ImGui::PushID(entityID);

				const auto& componentInfos = en::ComponentManager::GetComponentInfos();
				static std::vector<en::U32> hasNot;
				hasNot.clear();
				const auto endItr = componentInfos.cend();
				for (auto itr = componentInfos.cbegin(); itr != endItr; ++itr)
				{
					const auto& ci = itr->second;
					if (HasComponent(object, ci.enttID))
					{
						ImGui::PushID(ci.enttID);
						if (ImGui::Button("-"))
						{
							ci.remove(object.GetRegistry(), object.GetEntity());
							ImGui::PopID();
							continue;
						}
						else
						{
							ImGui::SameLine();
							if (ci.editor(object.GetRegistry(), object.GetEntity()))
							{
								result = true;
							}
							ImGui::PopID();
						}
					}
					else
					{
						hasNot.push_back(itr->first);
					}
				}

				if (!hasNot.empty())
				{
					if (ImGui::Button("+ Add Component")) 
					{
						ImGui::OpenPopup("Add Component");
					}

					if (ImGui::BeginPopup("Add Component")) 
					{
						ImGui::TextUnformatted("Available:");
						ImGui::Separator();
						for (auto componentHash : hasNot)
						{
							const auto& ci = componentInfos.at(componentHash);
							ImGui::PushID(componentHash);
							if (ImGui::Selectable(ci.name)) 
							{
								ci.add(object.GetRegistry(), object.GetEntity());
							}
							ImGui::PopID();
						}
						ImGui::EndPopup();
					}
				}

				ImGui::PopID();
			}
			else
			{
				ImGui::Text("Invalid entity");
			}
			ImGui::Unindent();
		}
		return true;
	}

private:
	static bool HasComponent(const en::Entity& entity, en::U32 enttComponentID)
	{
		const ComponentTypeID type[] = { enttComponentID };
		return entity.GetRegistry().runtime_view(std::cbegin(type), std::cend(type)).contains(entity.GetEntity());
	}
};
