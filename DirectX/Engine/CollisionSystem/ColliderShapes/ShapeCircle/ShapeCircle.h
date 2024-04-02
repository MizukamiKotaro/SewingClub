#pragma once
#include "Vector2.h"

class ShapeCircle {
public:
	ShapeCircle();

	void SetParameter(const Vector2& position, const Vector2& radius, const float& rotate = 0, const Vector2& velocity = {});
	void SetParameter(const Vector2& position, const float& radius, const float& rotate = 0, const Vector2& velocity = {});

public:
	Vector2 position_;
	Vector2 radius_;
	float rotate_;
	Vector2 velocity_;
	bool isCircle_;
};