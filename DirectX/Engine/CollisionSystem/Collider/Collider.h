#pragma once
#include "CollisionSystem/CollisionConfig.h"
#include "CollisionSystem/ColliderShapes/ShapeCircle/ShapeCircle.h"
#include <list>
#include <memory>

class CollisionManager;

class Collider {
public:
	virtual ~Collider() = default;

	virtual void OnCollision(const Collider& collider) = 0;

	void CreateCollider(ColliderShape shape, ColliderType type, ColliderMask mask, bool isBeDrived = false);

	void AddTargetMask(ColliderMask mask);

	const ColliderType GetType() const { return type_; }
	const ColliderShape GetShape() const { return shape_; }
	const ColliderMask GetMask() const { return mask_; }
	const std::list<ColliderMask>& GetTargetMasks() const { return targetMasks_; }
	const bool GetIsBeDrived() const { return isBeDrived_; }
	const bool GetIsHit() const { return isHit_; }
	void SetIsHit(bool isHit) { isHit_ = isHit; }

	ShapeCircle* GetCircle() const { return shapeCircle_.get(); }

	void SetCircle(const Vector2& position, const Vector2& radius, const float& rotate = 0.0f, const Vector2& velocity = {});
	void SetCircle(const Vector2& position, const float& radius, const float& rotate = 0.0f, const Vector2& velocity = {});

protected:
	CollisionManager* collisionManager_;

	std::unique_ptr<ShapeCircle> shapeCircle_;

	ColliderType type_;
	bool isBeDrived_; // めり込み処理をするとき動くか
	ColliderShape shape_;
	ColliderMask mask_;
	std::list<ColliderMask> targetMasks_;

	bool isHit_;
};