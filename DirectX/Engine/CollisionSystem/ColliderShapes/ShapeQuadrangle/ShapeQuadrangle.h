#pragma once
#include "Vector2.h"
#include "Vector3.h"

class ShapeQuadrangle {
public:
	ShapeQuadrangle() = default;

	void SetParameter(const Vector3& leftTop, const Vector3& rightTop, const Vector3& leftBottom, const Vector3& rightBottom);
	void SetParameter(const Vector2& leftTop, const Vector2& rightTop, const Vector2& leftBottom, const Vector2& rightBottom);

public:
	Vector2 leftTop_;
	Vector2 rightTop_;
	Vector2 leftBottom_;
	Vector2 rightBottom_;
};