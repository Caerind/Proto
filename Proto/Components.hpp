#pragma once

#include <Enlivengine/System/Meta.hpp>

#include <Enlivengine/Math/Vector2.hpp>

namespace en
{

struct PositionComponent
{
	PositionComponent() : position(0.0f, 0.0f) {}
	PositionComponent(F32 x, F32 y) : position(x, y) {}
	PositionComponent(const Vector2f& pPosition) : position(pPosition) {}

	Vector2f position;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::PositionComponent)
	ENLIVE_META_CLASS_MEMBER("position", &en::PositionComponent::position)
ENLIVE_META_CLASS_END()


namespace en
{

struct RotationComponent
{
	RotationComponent() : rotation(0.0f) {}
	RotationComponent(F32 pRotation) : rotation(pRotation) {}

	F32 rotation;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::RotationComponent)
	ENLIVE_META_CLASS_MEMBER("rotation", &en::RotationComponent::rotation)
ENLIVE_META_CLASS_END()

