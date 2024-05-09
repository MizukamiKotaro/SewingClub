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
	case ColliderShape::QUADRANGLE2D:
		shapeQuadrangle_ = std::make_unique<ShapeQuadrangle>();
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
	if (shape_ == ColliderShape::CIRCLE) {
		shapeCircle_->SetParameter(position, radius, rotate, velocity);
	}
}

void Collider::SetCircle(const Vector2& position, const float& radius, const float& rotate, const Vector2& velocity)
{
	if (shape_ == ColliderShape::CIRCLE) {
		shapeCircle_->SetParameter(position, radius, rotate, velocity);
	}
}

void Collider::SetQuadrangle(const Vector2& leftTop, const Vector2& rightTop, const Vector2& leftBottom, const Vector2& rightBottom)
{
	if (shape_ == ColliderShape::QUADRANGLE2D) {
		shapeQuadrangle_->SetParameter(leftTop, rightTop, leftBottom, rightBottom);
	}
}

void Collider::SetQuadrangle(const Vector3& leftTop, const Vector3& rightTop, const Vector3& leftBottom, const Vector3& rightBottom)
{
	if (shape_ == ColliderShape::QUADRANGLE2D) {
		shapeQuadrangle_->SetParameter(leftTop, rightTop, leftBottom, rightBottom);
	}
}
