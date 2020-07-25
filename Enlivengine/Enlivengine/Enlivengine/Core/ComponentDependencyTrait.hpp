#pragma once

template <typename T>
struct ComponentDependencyTrait
{
	static constexpr bool hasDependency = false;
};