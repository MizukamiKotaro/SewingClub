#include "ShapeCircle.h"

ShapeCircle::ShapeCircle()
{
	position_ = {};
	radius_ = {};
	rotate_ = 0.0f;
	velocity_ = {};
	isCircle_ = true;
}

void ShapeCircle::SetParameter(const Vector2& position, const Vector2& radius, const float& rotate, const Vector2& velocity)
{
	position_ = position;
	radius_ = radius;
	rotate_ = rotate;
	velocity_ = velocity;
	if (radius_.x == radius_.y) {
		isCircle_ = true;
	}
	else {
		isCircle_ = false;
	}
}

void ShapeCircle::SetParameter(const Vector2& position, const float& radius, const float& rotate, const Vector2& velocity)
{
	position_ = position;
	radius_ = { radius,radius };
	rotate_ = rotate;
	velocity_ = velocity;
	isCircle_ = true;
}