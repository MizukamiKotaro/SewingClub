#include "Collider.h"
#include <algorithm>
#include "CollisionSystem/CollisionManager/CollisionManager.h"

void Collider::CreateCollider(ColliderShape shape, ColliderType type, ColliderMask mask, bool isBeDrived)
{
	collisionManager_ = CollisionManager::GetInstance();

	shape_ = shape;
	type_ = type;
	mask_ = mask;
	isBeDrived_ = isBeDrived;

	switch (shape)
	{
	case ColliderShape::UNKNOWN:
		break;
	case ColliderShape::CIRCLE:
		shapeCircle_ = std::make_unique<ShapeCircle>();
		break;
	case ColliderShape::BOX2D:
		break;
	case ColliderShape::MAPCHIP2D:
		break;
	default:
		break;
	}
}

void Collider::AddTargetMask(ColliderMask mask)
{
	if (std::find(targetMasks_.begin(), targetMasks_.end(), mask) == targetMasks_.end()) {
		targetMasks_.push_back(mask);
	}
}

void Collider::SetCircle(const Vector2& position, const Vector2& radius, const float& rotate, const Vector2& velocity)
{
	shapeCircle_->SetParameter(position, radius, rotate, velocity);
}

void Collider::SetCircle(const Vector2& position, const float& radius, const float& rotate, const Vector2& velocity)
{
	shapeCircle_->SetParameter(position, radius, rotate, velocity);
}
