#include "WaterWave.h"
#include "Camera.h"
#include <algorithm>

std::unique_ptr<GlobalVariableUser> WaterWave::globalVariable_ = nullptr;

const std::vector<std::string> WaterWave::fNames = {
	"力が最大の時に動く最大の角度",
	"広がりきる時間",
	"力が最大になるときの速度",
	"最大の時の力",
};

std::vector<float> WaterWave::fParas_ = {
	1,1,1,1,
};

WaterWave::WaterWave(const Vector3& velocity, const float& rotate, const bool& isDown, const int& no)
{
	float speed = std::clamp(velocity.Length(), 0.0f, fParas_[kMaxSpeed]);

	power_ = fParas_[kMaxPower] * speed / fParas_[kMaxSpeed];
	firstRotate_ = rotate;
	lRotate_ = rotate;
	rRotate_ = rotate;
	time_ = 0.0f;
	preLRotate_ = rotate;
	preRRotate_ = rotate;
	maxRotate_ = fParas_[kConvertingMagnification] * power_ / fParas_[kMaxPower];

	if (isDown) {
		power_ *= -1;
	}
	nums_.push_back(no);
}

void WaterWave::StaticInitialize()
{
	globalVariable_ = std::make_unique<GlobalVariableUser>("Water", "Wave");
	globalVariable_->CreateGroup();

	SetGlobalVariable();
}

void WaterWave::Update(const float& deltaTime)
{
	preLRotate_ = lRotate_;
	preRRotate_ = rRotate_;

	time_ += deltaTime;
	time_ = std::clamp(time_, 0.0f, fParas_[kExpandTime]);
	float t = time_ / fParas_[kExpandTime];

	float rotate = t * maxRotate_;
	lRotate_ = firstRotate_ + rotate;
	rRotate_ = firstRotate_ - rotate;
}

void WaterWave::StaticUpdate()
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG
}

bool WaterWave::IsFinish() const
{
	if (maxRotate_ <= lRotate_ - firstRotate_) {
		return true;
	}
	return false;
}

float WaterWave::GetPower(const float& rotate) const
{
	if ((preLRotate_ <= rotate && rotate <= lRotate_) || (preRRotate_ >= rotate && rotate >= rRotate_)) {
		return power_ * (maxRotate_ - std::abs(rotate - firstRotate_)) / maxRotate_;
	}
	else if (preLRotate_ <= rotate + 6.28f && rotate + 6.28f <= lRotate_) {
		return power_ * (maxRotate_ - std::abs(rotate + 6.28f - firstRotate_)) / maxRotate_;
	}
	else if (preRRotate_ >= rotate - 6.28f && rotate - 6.28f >= rRotate_) {
		return power_ * (maxRotate_ - std::abs(rotate - 6.28f - firstRotate_)) / maxRotate_;
	}

	return 0.0f;
}

void WaterWave::SetGlobalVariable()
{
	for (int i = 0; i < kFloatEnd; i++) {
		globalVariable_->AddItem(fNames[i], fParas_[i]);
	}

	ApplyGlobalVariable();
}

void WaterWave::ApplyGlobalVariable()
{
	for (int i = 0; i < kFloatEnd; i++) {
		fParas_[i] = globalVariable_->GetFloatValue(fNames[i]);

		if (fParas_[i] < 0.0001f) {
			fParas_[i] = 0.0001f;
		}
	}
}