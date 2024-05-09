#include "CollisionCircleQuadrangle.h"
#include "CollisionSystem/Collider/Collider.h"
#include "CollisionSystem/ColliderShapes/ShapeCircle/ShapeCircle.h"
#include "CollisionSystem/ColliderShapes/ShapeQuadrangle/ShapeQuadrangle.h"
#include "calc.h"
#include <cmath>

bool CollisionCircleQuadrangle::IsCollision(Collider* a, Collider* b)
{

	ShapeCircle* circle = a->GetCircle();
	ShapeQuadrangle* quadrangle = b->GetQuadrangle();

	if (IsCollision(quadrangle->leftTop_, quadrangle->rightBottom_, quadrangle->rightTop_, circle->position_, circle->radius_.x) ||
		IsCollision(quadrangle->leftTop_, quadrangle->rightBottom_, quadrangle->leftBottom_, circle->position_, circle->radius_.x)) {
		if (a->GetType() == ColliderType::COLLIDER || b->GetType() == ColliderType::COLLIDER) {
			return true;
		}
		// めり込み処理保留
	}

	return false;
}

float CollisionCircleQuadrangle::MakeOuter(const Vector2& pos1, const Vector2& pos2, const Vector2& pos3)
{
	Vector2 vector[2] = { {pos2.x - pos1.x,pos2.y - pos1.y},{pos3.x - pos1.x,pos3.y - pos1.y} };
	return Calc::Outer(vector[0], vector[1]);
}

bool CollisionCircleQuadrangle::IsCollision(const Vector2& pos1, const Vector2& pos2, const Vector2& pos3, const Vector2& center, const float& radius)
{
	float len[3] = {};

	len[0] = Calc::MakeLength(center, pos1, pos2);
	len[1] = Calc::MakeLength(center, pos1, pos3);
	len[2] = Calc::MakeLength(center, pos2, pos3);

	float minLen = len[0];

	for (int i = 0; i < 2; i++) {
		if (minLen > len[i + 1]) {
			minLen = len[i + 1];
		}
	}

	if (minLen <= radius) {
		return true;
	}

	float outer[3] = {};

	outer[0] = MakeOuter(pos1, pos2, center);
	outer[1] = MakeOuter(pos2, pos3, center);
	outer[2] = MakeOuter(pos3, pos1, center);

	if ((outer[0] >= 0 && outer[1] >= 0 && outer[2] >= 0) || (outer[0] <= 0 && outer[1] <= 0 && outer[2] <= 0)) {
		return true;
	}
	return false;
}
