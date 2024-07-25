#pragma once
#include "Utils/Transform/Transform.h"
#include <memory>
#include <string>

class FollowCamera {
public:

	FollowCamera();

	void Initialize(const Vector3* target, const Vector3& upperlimit, const Vector3& lowerlimit, const float& posZ = 0.0f);

	Vector3 Update();

	void Reset();
	void SetFirstOffsetZ(const float& offsetZ); // Z座標のセット

private:
	void FollowUpdate();

private:

	const Vector3* target_ = nullptr;

	/*
	"カメラの遅延定数",
	"カメラのX座標制限",
	"カメラのY座標制限",
	*/
	float delayConstant_ = 0.0f; // 遅延定数
	float space_ = 0.0f; // 余白
	Vector3 upperLimitPos_; // 上限座標
	Vector3 lowerLimitPos_; // 下限座標

	// 追従対象の残像座標
	Vector3 interTarget_;
	float offset_;

	const std::string groupName_ = "FollowCamera";
};