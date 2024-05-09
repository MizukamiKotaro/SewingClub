#include "ShapeQuadrangle.h"

void ShapeQuadrangle::SetParameter(const Vector3& leftTop, const Vector3& rightTop, const Vector3& leftBottom, const Vector3& rightBottom)
{
	leftTop_ = { leftTop.x, leftTop.y };
	rightTop_ = { rightTop.x, rightTop.y };
	leftBottom_ = { leftBottom.x, leftBottom.y };
	rightBottom_ = { rightBottom.x, rightBottom.y };
}

void ShapeQuadrangle::SetParameter(const Vector2& leftTop, const Vector2& rightTop, const Vector2& leftBottom, const Vector2& rightBottom)
{
	leftTop_ = leftTop;
	rightTop_ = rightTop;
	leftBottom_ = leftBottom;
	rightBottom_ = rightBottom;
}
