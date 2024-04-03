#include "GravityAreaSearch.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "calc.h"

GravityAreaSearch::GravityAreaSearch()
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::GRAVITY_AREA_SEARCH);
	Collider::AddTargetMask(ColliderMask::GRAVITY_AREA);
	nearPos_ = {};
	length_ = 0.0f;
	nearDotPos_ = {};
	dot_ = -1.0f;
}

void GravityAreaSearch::Update(const Vector3& pos, const Vector3& velocity)
{
	nearPos_ = {};
	length_ = 0.0f;
	nearDotPos_ = {};
	dot_ = -1.0f;
	Vector2 vect = { velocity.x,velocity.y };
	vect = vect.Normalize();
	Collider::SetCircle({ pos.x,pos.y }, 10000.0f, 0.0f, vect);
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
		float dot = Calc::Dot((collider.GetCircle()->position_ - GetCircle()->position_).Normalize(), GetCircle()->velocity_);
		if (dot_ == -1.0f || dot_ < dot) {
			nearDotPos_ = collider.GetCircle()->position_;
			dot_ = dot;
		}
	}
}