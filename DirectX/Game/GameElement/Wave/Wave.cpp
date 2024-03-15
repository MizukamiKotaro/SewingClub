#include "Wave.h"
#include "Camera.h"
#include <algorithm>
#include "FrameInfo/FrameInfo.h"

std::unique_ptr<GlobalVariableUser> Wave::globalVariable_ = nullptr;
FrameInfo* Wave::frameInfo_ = nullptr;

const std::string Wave::fNames[kFloatEnd] = {
	"パワーから最大半径への変換倍率",
	"広がりきる時間"
};

float Wave::fParas_[kFloatEnd] = { 1.0f };

Wave::Wave(const Vector3& position, float power, float radius)
{
	position_ = position;
	power_ = power;
	maxRadius_ = power * fParas_[kConvertingMagnification];
	firstRadius_ = radius;
	radius_ = 0.0f;
	preRadius_ = 0.0f;
	time_ = 0.0f;
}

void Wave::StaticInitialize()
{
	frameInfo_ = FrameInfo::GetInstance();

	globalVariable_ = std::make_unique<GlobalVariableUser>("Wave", "StaticWave");
	globalVariable_->CreateGroup();

	SetGlobalVariable();
}

void Wave::Update()
{
	preRadius_ = radius_;

	time_ += frameInfo_->GetDeltaTime();
	time_ = std::clamp(time_, 0.0f, fParas_[kExpandTime]);
	float t = time_ / fParas_[kExpandTime];

	radius_ = (1.0f - t) * firstRadius_ + t * maxRadius_;
}

void Wave::StaticUpdate()
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG
}

bool Wave::IsFinish() const 
{
	if (maxRadius_ <= radius_) {
		return true;
	}
	return false;
}

void Wave::SetGlobalVariable()
{
	for (int i = 0; i < kFloatEnd; i++) {
		globalVariable_->AddItem(fNames[i], fParas_[i]);
	}

	ApplyGlobalVariable();
}

void Wave::ApplyGlobalVariable()
{
	for (int i = 0; i < kFloatEnd; i++) {
		fParas_[i] = globalVariable_->GetFloatValue(fNames[i]);

		if (fParas_[i] < 0.0001f) {
			fParas_[i] = 0.0001f;
		}
	}
}