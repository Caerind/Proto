#include "EntityManager.hpp"

namespace en
{

EntityManager::EntityManager()
{
}

Entity EntityManager::CreateEntity()
{
	return Entity(*this, mRegistry.create());
}

void EntityManager::DestroyEntity(Entity& entity)
{
	if (entity.IsValid())
	{
		mRegistry.destroy(entity.GetEntity());
		entity.mEntity = entt::null;
	}
}

void EntityManager::ClearEntities()
{
	mRegistry.clear();
}

} // namespace en