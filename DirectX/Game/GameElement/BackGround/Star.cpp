#include "Star.h"
#include <algorithm>

void Star::Initialize(const Vector3& position) {
	transform_.translate_ = position;
	transform_.scale_ = Vector3(0.0f, 0.0f, 1.0f);
	isScaling_ = false;
}

bool Star::Update() {
	bool flag = ScaleUpDown();
	transform_.translate_.z = 0.0f;
	transform_.UpdateMatrix();
	return flag;
}

bool Star::ScaleUpDown() {
	float speed_ = 0.01f; // 拡縮速度
	if (isScaling_) {
		speed_ = -0.01f;
	}
	transform_.scale_.x = std::clamp(transform_.scale_.x += speed_, 0.0f, kMaxSize_);
	transform_.scale_.y = std::clamp(transform_.scale_.y += speed_, 0.0f, kMaxSize_);
	if (transform_.scale_.x >= kMaxSize_) {
		isScaling_ = true;
	}
	else if (transform_.scale_.x <= 0.0f) {
		return true; // 縮小しきったら座標を変更するためにtrueを返す
	}
	return false;
}
