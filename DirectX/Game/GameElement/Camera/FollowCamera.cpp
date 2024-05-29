#include "FollowCamera.h"
#include "GlobalVariables/GlobalVariables.h"
#include "calc.h"
#include <algorithm>

FollowCamera::FollowCamera()
{
	interTarget_ = {};

	delayConstant_ = 0.1f;

	Reset();

}

void FollowCamera::Initialize(const Vector3* target, const Vector3& upperlimit, const Vector3& lowerlimit, const float& posZ)
{
	target_ = target;
	upperLimitPos_ = upperlimit;
	lowerLimitPos_ = lowerlimit;
	offset_ = posZ;
	Reset();

}

Vector3 FollowCamera::Update() {

	FollowUpdate();
	return interTarget_;
}

void FollowCamera::Reset()
{
	// 追従対象がいれば
	if (target_) {
		// 追従座標の初期化
		Vector3 pos = *target_;

		interTarget_ = pos;
		interTarget_.z = offset_;
	}
}

void FollowCamera::SetFirstOffsetZ(const float& offsetZ) {
	offset_ = offsetZ;
}

void FollowCamera::FollowUpdate()
{
	if (target_) {
		Vector3 pos = *target_;
		
		// 制限
		pos.x = std::clamp(pos.x, lowerLimitPos_.x - space_, upperLimitPos_.x + space_);
		pos.y = std::clamp(pos.y, lowerLimitPos_.y - space_, upperLimitPos_.y + space_);
		pos.z = offset_;

		interTarget_ = Calc::Lerp(interTarget_, pos, delayConstant_);
		interTarget_.z = Calc::Lerp(interTarget_.z, offset_, delayConstant_);
	}
}
