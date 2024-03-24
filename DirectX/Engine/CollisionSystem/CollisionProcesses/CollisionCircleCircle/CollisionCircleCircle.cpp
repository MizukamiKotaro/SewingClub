#include "CollisionCircleCircle.h"
#include "CollisionSystem/Collider/Collider.h"
#include "CollisionSystem/ColliderShapes/ShapeCircle/ShapeCircle.h"
#include <cmath>

bool CollisionCircleCircle::IsCollision(Collider* a, Collider* b)
{
	ShapeCircle* aCircle = a->GetCircle();
	ShapeCircle* bCircle = b->GetCircle();

	if (aCircle->isCircle_ && bCircle->isCircle_) {
		if ((aCircle->position_ - bCircle->position_).Length() <= aCircle->radius_.x + bCircle->radius_.x) {
			if (a->GetType() == ColliderType::COLLIDER || b->GetType() == ColliderType::COLLIDER) {
				return true;
			}
			// めり込み処理保留
		}
		return false;
	}

	//if (aCircle->radius_.x == 0.0f && aCircle->radius_.y == 0.0f) {
	//	if (bCircle->radius_.x == 0.0f && bCircle->radius_.y == 0.0f) {
	//		return false;
	//	}
	//	if (bCircle->radius_.y == 0.0f) {
	//		// 点と線のあたり判定保留
	//		return false;
	//	}
	//	Vector2 ofset = aCircle->position_ - bCircle->position_;
	//	Vector2 pos = {};
	//	pos.x = ofset.x * std::cosf(bCircle->rotate_) + ofset.y * std::sinf(bCircle->rotate_);
	//	pos.y = bCircle->radius_.x / bCircle->radius_.y * (-ofset.x * std::sinf(bCircle->rotate_) + ofset.y * std::cosf(bCircle->rotate_));

	//	if (pos.x * pos.x + pos.y * pos.y <= bCircle->radius_.x * bCircle->radius_.y) {
	//		if (a->GetType() == ColliderType::COLLIDER || b->GetType() == ColliderType::COLLIDER) {
	//			return true;
	//		}
	//		else {
	//			// めり込み処理をする場合の処理保留

	//			return true;
	//		}
	//	}
	//	return false;
	//}
	//if (bCircle->radius_.x == 0.0f && bCircle->radius_.y == 0.0f) {
	//	if (aCircle->radius_.x == 0.0f && aCircle->radius_.y == 0.0f) {
	//		return false;
	//	}
	//	if (aCircle->radius_.y == 0.0f) {
	//		// 点と線のあたり判定保留
	//		return false;
	//	}
	//	Vector2 ofset = bCircle->position_ - aCircle->position_;
	//	Vector2 pos = {};
	//	pos.x = ofset.x * std::cosf(aCircle->rotate_) + ofset.y * std::sinf(aCircle->rotate_);
	//	pos.y = aCircle->radius_.x / aCircle->radius_.y * (-ofset.x * std::sinf(aCircle->rotate_) + ofset.y * std::cosf(aCircle->rotate_));

	//	if (pos.x * pos.x + pos.y * pos.y <= aCircle->radius_.x * aCircle->radius_.y) {
	//		if (a->GetType() == ColliderType::COLLIDER || b->GetType() == ColliderType::COLLIDER) {
	//			return true;
	//		}
	//		else {
	//			// めり込み処理をする場合の処理保留

	//			return true;
	//		}
	//	}
	//	return false;
	//}

	//// 楕円と楕円前提 線は考慮してない


	return false;
}
