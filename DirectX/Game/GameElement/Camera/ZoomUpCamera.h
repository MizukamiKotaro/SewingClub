#pragma once
#include "Math/Vector3.h"

class ZoomUpCamera {
public:
	ZoomUpCamera() = default;
	~ZoomUpCamera() = default;

	void Initialize(const Vector3& oldCameraPos, const Vector3& goalPos);

	Vector3 Update(const float& frameMaxTime, const float& delta);
	bool GetFinish() const { return isFinished_; }

private:
	Vector3 oldPosition_;
	Vector3 goalPosition_;
	float count_;
	bool isFinished_ = false;

};
