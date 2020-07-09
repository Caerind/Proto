#pragma once

#include <entt/entt.hpp>

#include "Entity.hpp"

namespace en
{

class EntityManager
{
public:
	EntityManager();

	Entity CreateEntity();
	void DestroyEntity(Entity& entity);
	void ClearEntities();

	template <typename Func>
	void Each(Func func) const
	{
		mRegistry.each(func);
	}

	template <typename... Component, typename... Exclude>
	auto View(entt::exclude_t<Exclude...> exclusion = {}) const
	{
		return mRegistry.view<Component...>(exclusion);
	}

	template <typename... Component, typename... Exclude>
	auto View(entt::exclude_t<Exclude...> exclusion = {})
	{
		return mRegistry.view<Component...>(exclusion);
	}

private:
	friend class Entity;

	entt::registry& GetRegistry() { return mRegistry; }
	const entt::registry& GetRegistry() const { return mRegistry; }

private:
	entt::registry mRegistry;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::EntityManager)
ENLIVE_META_CLASS_END()

// TODO : Figure out how to move it elsewhere
template <>
struct CustomImGuiEditor<en::EntityManager>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::EntityManager& object, const char* name)
	{
		assert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			if (ImGui::Button("New Entity"))
			{
				object.CreateEntity();
				modified = true;
			}
			object.Each([&object, &modified](auto entityEntt)
			{
				en::Entity entity(object, entityEntt);
				if (entity.IsValid())
				{
					ImGui::PushID(entity.GetID());
					if (ImGui::Button("x"))
					{
						object.DestroyEntity(entity);
						modified = true;
					}
					else
					{
						ImGui::SameLine();
						if (CustomImGuiEditor<en::Entity>::ImGuiEditor(entity, entity.GetName()))
						{
							modified = true;
						}
					}
					ImGui::PopID();
				}
			});
		}
		return modified;
	}
};