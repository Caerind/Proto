#pragma once

#include <entt/entt.hpp>

#include <Enlivengine/System/Meta.hpp>

#include <Enlivengine/Core/ComponentManager.hpp>

namespace en
{

class EntityManager;
class World;

// This is only a handle of an entity in a registry
// It does not create or destroy entity from registry
// It is used for CustomEditor/Serialization and convenience
class Entity
{
public:
	Entity();
	Entity(EntityManager& manager, entt::entity entity);
	Entity(World& world, entt::entity entity);

	bool IsValid() const;
	U32 GetID() const;

	const char* GetName() const;

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

	World& GetWorld();
	const World& GetWorld() const;

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
	enAssert(ComponentManager::IsRegistered<T>());
	enAssert(IsValid());

	return GetRegistry().assign<T>(mEntity); 
}

template <typename T, typename ...Args>
T& Entity::Add(Args&& ...args)
{
	enAssert(ComponentManager::IsRegistered<T>());
	enAssert(IsValid());
	
	return GetRegistry().emplace<T>(mEntity, std::forward<Args>(args)...); 
}

template <typename ...Components> 
bool Entity::Has() const 
{ 
	enAssert(IsValid());

	return GetRegistry().has<Components...>(mEntity); 
}

template <typename T> 
void Entity::Remove()
{
	enAssert(ComponentManager::IsRegistered<T>());
	enAssert(IsValid());
	enAssert(Has<T>());
	
	GetRegistry().remove<T>(mEntity); 
}

template <typename T> 
T& Entity::Get()
{
	enAssert(ComponentManager::IsRegistered<T>());
	enAssert(IsValid());
	enAssert(Has<T>());

	return GetRegistry().get<T>(mEntity); 
}

template <typename T> 
const T& Entity::Get() const 
{
	enAssert(ComponentManager::IsRegistered<T>());
	enAssert(IsValid());
	enAssert(Has<T>());

	return GetRegistry().get<T>(mEntity); 
}

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Entity)
ENLIVE_META_CLASS_END()

// TODO : Figure out how to move it elsewhere
template <>
struct CustomObjectEditor<en::Entity>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Entity& object, const char* name)
	{
		const bool fromEntityBrowser = name == nullptr;
		bool display = true;

		if (!fromEntityBrowser)
		{
			display = ImGui::CollapsingHeader(name);
		}

		if (display)
		{
			bool result = false;
			if (!fromEntityBrowser)
			{
				ImGui::Indent();
			}
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
			if (!fromEntityBrowser)
			{
				ImGui::Unindent();
			}
		}
		return true;
	}

private:
	using ComponentTypeID = ENTT_ID_TYPE;
	static bool HasComponent(const en::Entity& entity, ComponentTypeID enttComponentID)
	{
		// TODO : Factorize with CustomXmlSerialization<en::Entity>
		const ComponentTypeID type[] = { enttComponentID };
		return entity.GetRegistry().runtime_view(std::cbegin(type), std::cend(type)).contains(entity.GetEntity());
	}
};

// TODO : Figure out how to move it elsewhere
template <>
struct CustomXmlSerialization<en::Entity>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Entity& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Entity>();
			const auto& componentInfos = en::ComponentManager::GetComponentInfos();
			const auto endItr = componentInfos.cend();
			for (auto itr = componentInfos.cbegin(); itr != endItr; ++itr)
			{
				const auto& ci = itr->second;
				if (HasComponent(object, ci.enttID))
				{
					ci.serialize(dataFile, object.GetRegistry(), object.GetEntity());
				}
			}
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Entity& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (strcmp(name, "") == 0)
		{
			bool anyError = false;

			// We should already be on the node of the entity : See explanation on EntityManager::Deserialize
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Entity>::GetHash());
			if (parser.ReadFirstNode())
			{
				static std::vector<DeserializationComponentNode> componentNodes; // TODO : Move to Array
				componentNodes.clear();
				do
				{
					const std::string nodeName = parser.GetNodeName();
					const en::U32 nodeNameHash = en::Hash::ConstexprHash(nodeName);
					const en::U32 nodeType = dataFile.ReadCurrentType();
					const bool registeredComponent = en::ComponentManager::IsRegistered(nodeNameHash);
					if (nodeNameHash == nodeType && registeredComponent)
					{
						componentNodes.push_back({ nodeName, nodeNameHash });
					}
					else
					{
						if (nodeNameHash != nodeType)
						{
							enLogWarning(en::LogChannel::Core, "Incompatible component : {}({}) <-> {} ?", nodeNameHash, nodeName, nodeType);
						}
						if (!registeredComponent)
						{
							enLogWarning(en::LogChannel::Core, "Unregistered component : {}", nodeName);
						}
						anyError = true;
					}
				} while (parser.NextSibling());
				parser.CloseNode();

				// Now, we are back at the entity level, parse components
				for (const auto& componentNode : componentNodes)
				{
					const auto& componentInfos = en::ComponentManager::GetComponentInfos();
					enAssert(componentInfos.find(componentNode.hash) != componentInfos.end());
					if (!componentInfos.at(componentNode.hash).deserialize(dataFile, object.GetRegistry(), object.GetEntity()))
					{
						anyError = true;
					}
				}
			}
			return !anyError;
		}
		else
		{
			// Single entity deserialization is not supported yet
			enAssert(false);
			return false;
		}
	}

private:
	struct DeserializationComponentNode
	{
		std::string name;
		en::U32 hash;
	};

	using ComponentTypeID = ENTT_ID_TYPE;
	static bool HasComponent(const en::Entity& entity, ComponentTypeID enttComponentID)
	{
		// TODO : Factorize with CustomObjectEditor<en::Entity>
		const ComponentTypeID type[] = { enttComponentID };
		return entity.GetRegistry().runtime_view(std::cbegin(type), std::cend(type)).contains(entity.GetEntity());
	}
};