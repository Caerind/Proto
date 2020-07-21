#include "Entity.hpp"

#include "EntityManager.hpp"

#include "CoreComponents.hpp"

namespace en
{

Entity::Entity()
	: mManager(nullptr)
	, mEntity(entt::null)
{
}

Entity::Entity(EntityManager& manager, entt::entity entity)
	: mManager(&manager)
	, mEntity(entity)
{
}

bool Entity::IsValid() const
{
	return mManager != nullptr && GetRegistry().valid(mEntity);
}

U32 Entity::GetID() const
{
	return static_cast<U32>(entt::to_integral(mEntity));
}

#ifdef ENLIVE_DEBUG
const char* Entity::GetName() const
{
	if (!IsValid())
	{
		return "";
	}
	else if (Has<NameComponent>())
	{
		return Get<NameComponent>().name.c_str();
	}
	else
	{
		return "";
	}
}
#endif // ENLIVE_DEBUG

bool Entity::HasManager() const
{
	return mManager != nullptr;
}

EntityManager& Entity::GetManager()
{
	enAssert(mManager != nullptr);
	return *mManager;
}

const EntityManager& Entity::GetManager() const
{
	enAssert(mManager != nullptr);
	return *mManager;
}

const entt::entity& Entity::GetEntity() const
{
	return mEntity;
}

entt::registry& Entity::GetRegistry()
{
	enAssert(mManager != nullptr);
	return mManager->GetRegistry();
}

const entt::registry& Entity::GetRegistry() const
{
	enAssert(mManager != nullptr);
	return mManager->GetRegistry();
}

} // namespace en