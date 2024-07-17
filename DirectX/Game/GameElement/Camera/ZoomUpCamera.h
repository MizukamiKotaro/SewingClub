#pragma once
#include "Math/Vector3.h"

class ZoomUpCamera {
public:
	ZoomUpCamera() = default;
	~ZoomUpCamera() = default;

	void Initialize(const Vector3& oldCameraPos, const Vector3& goalPos);

	Vector3 Update(const float& frameMaxTime, const float& delta);

private:
	Vector3 oldPosition_;
	Vector3 goalPosition_;
	float count_;

};
