#pragma once

#include <Enlivengine/System/Meta.hpp>

#include <string>


namespace en
{

struct NameComponent
{
	NameComponent() : name("") {}
	NameComponent(const char* pName) : name(pName) {}
	NameComponent(const std::string& pName) : name(pName) {}

	std::string name;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::NameComponent)
	ENLIVE_META_CLASS_MEMBER("name", &en::NameComponent::name)
ENLIVE_META_CLASS_END()

