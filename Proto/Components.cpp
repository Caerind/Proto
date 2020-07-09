#include "Components.hpp"

namespace en
{

std::unordered_map<U32, ComponentManager::ComponentInfo> ComponentManager::mComponents;

const char* Component::GetStaticClassName()
{
	return TypeInfo<Component>::GetName();
}

U32 Component::GetStaticClassHash()
{
	return TypeInfo<Component>::GetHash();
}

#ifdef ENLIVE_DEBUG

const char* NameComponent::GetStaticClassName()
{
	return TypeInfo<NameComponent>::GetName();
}

U32 NameComponent::GetStaticClassHash()
{
	return TypeInfo<NameComponent>::GetHash();
}

#endif // ENLIVE_DEBUG

const char* PositionComponent::GetStaticClassName()
{
	return TypeInfo<PositionComponent>::GetName();
}

U32 PositionComponent::GetStaticClassHash()
{
	return TypeInfo<PositionComponent>::GetHash();
}

} // namespace en