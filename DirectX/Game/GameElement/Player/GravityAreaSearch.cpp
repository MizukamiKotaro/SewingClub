#include "GravityAreaSearch.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"

GravityAreaSearch::GravityAreaSearch()
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::GRAVITY_AREA_SEARCH);
	Collider::AddTargetMask(ColliderMask::GRAVITY_AREA);
	nearPos_ = {};
	length_ = 0.0f;
}

void GravityAreaSearch::Update(const Vector3& pos)
{
	nearPos_ = {};
	length_ = 0.0f;
	Collider::SetCircle({ pos.x,pos.y }, 100.0f);
	collisionManager_->SetCollider(this);
}

void GravityAreaSearch::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::GRAVITY_AREA) {
		float length = (collider.GetCircle()->position_ - GetCircle()->position_).Length();
		if (length_ == 0.0f || length_ > length) {
			nearPos_ = collider.GetCircle()->position_;
			length_ = length;
		}
	}
}