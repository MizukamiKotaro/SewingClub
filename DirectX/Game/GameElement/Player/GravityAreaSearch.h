#pragma once
#include "CollisionSystem/Collider/Collider.h"
#include "Vector3.h"
#include "Vector2.h"

class GravityAreaSearch : public Collider
{
public:
	GravityAreaSearch();

	void Update(const Vector3& pos);

	const Vector2& GetNearPos() const { return nearPos_; }

private:
	void OnCollision(const Collider& collider) override;
	Vector2 nearPos_;
	float length_;
};
