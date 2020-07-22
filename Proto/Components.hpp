#pragma once

#include <Enlivengine/System/Meta.hpp>

#include <Enlivengine/Math/Vector2.hpp>

struct PositionComponent
{
	PositionComponent() : position(0.0f, 0.0f) {}
	PositionComponent(en::F32 x, en::F32 y) : position(x, y) {}
	PositionComponent(const en::Vector2f& pPosition) : position(pPosition) {}

	en::Vector2f position;
};

ENLIVE_META_CLASS_BEGIN(PositionComponent)
	ENLIVE_META_CLASS_MEMBER("position", &PositionComponent::position)
ENLIVE_META_CLASS_END()


struct RotationComponent
{
	RotationComponent() : rotation(0.0f) {}
	RotationComponent(en::F32 pRotation) : rotation(pRotation) {}

	en::F32 rotation;
};

ENLIVE_META_CLASS_BEGIN(RotationComponent)
	ENLIVE_META_CLASS_MEMBER("rotation", &RotationComponent::rotation)
ENLIVE_META_CLASS_END()

