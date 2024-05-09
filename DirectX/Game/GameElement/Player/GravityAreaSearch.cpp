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
		if (collider.GetShape() == ColliderShape::CIRCLE) {
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
		else if (collider.GetShape() == ColliderShape::QUADRANGLE2D) {
			ShapeQuadrangle* quadrangle = collider.GetQuadrangle();
			Vector2 startPos = (quadrangle->leftTop_ + quadrangle->leftBottom_) / 2;
			Vector2 endPos = (quadrangle->rightTop_ + quadrangle->rightBottom_) / 2;
			Vector3 vect = { endPos.x - startPos.x, endPos.y - startPos.y, 0.0f };
			Vector3 point = Calc::ClosestPoint({ GetCircle()->position_.x,GetCircle()->position_.y,0.0f }, Segment{ {startPos.x,startPos.y,0.0f},vect });
			Vector2 position = { point.x,point.y };

			float length = (position - GetCircle()->position_).Length();
			if (length_ == 0.0f || length_ > length) {
				nearPos_ = position;
				length_ = length;
			}
			float dot = Calc::Dot((position - GetCircle()->position_).Normalize(), GetCircle()->velocity_);
			if (dot_ == -1.0f || dot_ < dot) {
				nearDotPos_ = position;
				dot_ = dot;
			}
		}
	}
}