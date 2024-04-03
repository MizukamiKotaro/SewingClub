#pragma once
#include "CollisionSystem/Collider/Collider.h"
#include "Vector3.h"
#include "Vector2.h"

class GravityAreaSearch : public Collider
{
public:
	GravityAreaSearch();

	void Update(const Vector3& pos, const Vector3& velocity = {});

	const Vector2& GetNearPos() const { return nearPos_; }
	const Vector2& GetNearDotPos() const { return nearDotPos_; }

private:
	void OnCollision(const Collider& collider) override;
	Vector2 nearPos_;
	float length_;
	Vector2 nearDotPos_;
	float dot_;
};
