#include "HitStop.h"

float HitStop::time_;
float HitStop::endTime_;
std::unique_ptr<GlobalVariableUser> HitStop::global_ = nullptr;
std::vector<const char*> HitStop::fPara_;

void HitStop::Initialize()
{
	time_ = 0.0f;
	endTime_ = 0.0f;
	if (!global_) {
		size_t n = static_cast<size_t>(HitStopType::kHitStopEnd);
		fPara_.resize(n);
		fPara_ = {
			"赤ちゃんジャンプの停止時間",
			"赤ちゃんキャッチの停止時間",
			"コンボ時の停止時間",
		};
		global_ = std::make_unique<GlobalVariableUser>("HitStopManager", "HitStop", "調整項目");
		for (size_t i = 0; i < n; i++) {
			global_->AddItem(fPara_[i], 0.1f);
		}
	}
}

void HitStop::Update(float& deltaTime)
{
	if (endTime_ != 0.0f) {
		time_ += deltaTime;
		if (time_ >= endTime_) {
			time_ = 0.0f;
			endTime_ = 0.0f;
		}
		else {
			deltaTime = 0.0f;
		}
	}
}

void HitStop::SetHitStop(const HitStopType& type)
{
	if (global_ && endTime_ == 0.0f) {
		endTime_ = global_->GetFloatValue(fPara_[static_cast<size_t>(type)]);
	}
}
