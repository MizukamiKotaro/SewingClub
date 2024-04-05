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

	//// STEP1 : E2を単位円にする変換をE1に施す
	//float DefAng = aCircle->rotate_ - bCircle->rotate_;
	//float Cos = std::cosf(DefAng);
	//float Sin = std::sinf(DefAng);
	//float nx = bCircle->radius_.x * Cos;
	//float ny = -bCircle->radius_.x * Sin;
	//float px = bCircle->radius_.y * Sin;
	//float py = bCircle->radius_.y * Cos;
	//float ox = std::cosf(aCircle->rotate_) * (bCircle->position_.x - aCircle->position_.x) + std::sinf(aCircle->rotate_) * (bCircle->position_.y - aCircle->position_.y);
	//float oy = -std::sinf(aCircle->rotate_) * (bCircle->position_.x - aCircle->position_.x) + std::cosf(aCircle->rotate_) * (bCircle->position_.y - aCircle->position_.y);

	//// STEP2 : 一般式A～Gの算出
	//float rx_pow2 = 1 / (aCircle->radius_.x * aCircle->radius_.x);
	//float ry_pow2 = 1 / (aCircle->radius_.y * aCircle->radius_.y);
	//float A = rx_pow2 * nx * nx + ry_pow2 * ny * ny;
	//float B = rx_pow2 * px * px + ry_pow2 * py * py;
	//float D = 2 * rx_pow2 * nx * px + 2 * ry_pow2 * ny * py;
	//float E = 2 * rx_pow2 * nx * ox + 2 * ry_pow2 * ny * oy;
	//float F = 2 * rx_pow2 * px * ox + 2 * ry_pow2 * py * oy;
	//float G = (ox / aCircle->radius_.x) * (ox / aCircle->radius_.x) + (oy / aCircle->radius_.y) * (oy / aCircle->radius_.y) - 1;

	//// STEP3 : 平行移動量(h,k)及び回転角度θの算出
	//float tmp1 = 1 / (D * D - 4 * A * B);
	//float h = (F * D - 2 * E * B) * tmp1;
	//float k = (E * D - 2 * A * F) * tmp1;
	//float Th = (B - A) == 0 ? 0 : atan(D / (B - A)) * 0.5f;

	//// STEP4 : +1楕円を元に戻した式で当たり判定
	//float CosTh = cos(Th);
	//float SinTh = sin(Th);
	//float A_tt = A * CosTh * CosTh + B * SinTh * SinTh - D * CosTh * SinTh;
	//float B_tt = A * SinTh * SinTh + B * CosTh * CosTh + D * CosTh * SinTh;
	//float KK = A * h * h + B * k * k + D * h * k - E * h - F * k + G;
	//if (KK > 0) KK = 0; // 念のため
	//float Rx_tt = 1 + sqrt(-KK / A_tt);
	//float Ry_tt = 1 + sqrt(-KK / B_tt);
	//float x_tt = CosTh * h - SinTh * k;
	//float y_tt = SinTh * h + CosTh * k;
	//float JudgeValue = x_tt * x_tt / (Rx_tt * Rx_tt) + y_tt * y_tt / (Ry_tt * Ry_tt);

	//if (JudgeValue <= 1) {
	//	return true; // 衝突
	//}

	return false;
}
