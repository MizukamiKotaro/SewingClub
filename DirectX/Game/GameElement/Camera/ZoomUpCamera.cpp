#include "ZoomUpCamera.h"
#include "Ease/Ease.h"

void ZoomUpCamera::Initialize(const Vector3& oldCameraPos, const Vector3& goalPos) {
	count_ = 0.0f;
	oldPosition_ = oldCameraPos;
	goalPosition_ = goalPos;
	goalPosition_.z = -20.0f;
}

Vector3 ZoomUpCamera::Update(const float& frameMaxTime, const float& delta) {
	Vector3 result{};

	const float kMaxFrame = frameMaxTime * 0.75f;
	result = Ease::UseEase(oldPosition_, goalPosition_, count_, kMaxFrame, Ease::EaseType::EaseOutBack);
	count_ += delta;
	if (kMaxFrame <= count_) {
		count_ = kMaxFrame;
	}

	return result;
}
