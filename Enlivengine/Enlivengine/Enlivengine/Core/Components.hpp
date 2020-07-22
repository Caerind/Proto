#pragma once

#include <Enlivengine/System/Meta.hpp>

#include <string>
#include <Enlivengine/Core/Transform.hpp>

namespace en
{

struct NameComponent
{
	NameComponent() : name("") {}
	NameComponent(const char* pName) : name(pName) {}
	NameComponent(const std::string& pName) : name(pName) {}

	std::string name;
};

struct TransformComponent
{
	TransformComponent() : transform() {}
	TransformComponent(const Transform& pTransform) : transform(pTransform) {}

	Transform transform;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::NameComponent)
	ENLIVE_META_CLASS_MEMBER("name", &en::NameComponent::name)
	ENLIVE_META_CLASS_END()

ENLIVE_META_CLASS_BEGIN(en::TransformComponent)
	ENLIVE_META_CLASS_MEMBER("transform", &en::TransformComponent::transform)
ENLIVE_META_CLASS_END()

