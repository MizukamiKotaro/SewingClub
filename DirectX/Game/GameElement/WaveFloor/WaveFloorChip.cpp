#include "WaveFloorChip.h"
#include "Camera.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "FrameInfo/FrameInfo.h"

InstancingModelManager* WaveFloorChip::instancingManager_ = nullptr;
const ModelData* WaveFloorChip::modelData_ = nullptr;
FrameInfo* WaveFloorChip::frameInfo_ = nullptr;

std::unique_ptr<GlobalVariableUser> WaveFloorChip::globalVariable_ = nullptr;

const std::string WaveFloorChip::fNames[kFloatEnd] = {
	"減速率",
	"最低速度",
	"スケール",
	"波の1往復の時間"
};

float WaveFloorChip::fParas_[kFloatEnd] = { 1.0f };

Vector3 WaveFloorChip::scale_ = { WaveFloorChip::fParas_[WaveFloorChip::kScale],WaveFloorChip::fParas_[WaveFloorChip::kScale] ,WaveFloorChip::fParas_[WaveFloorChip::kScale] };

WaveFloorChip::WaveFloorChip(const Vector3& position)
{
	position_ = position;
	ganeratePosition_ = position;
	velocity_ = {};
	rotate_ = Quaternion::Identity();
}

void WaveFloorChip::StaticInitialize()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	modelData_ = ModelDataManager::GetInstance()->LoadObj("Cube");
	frameInfo_ = FrameInfo::GetInstance();

	globalVariable_ = std::make_unique<GlobalVariableUser>("Wave", "FloorChip");
	globalVariable_->CreateGroup();

	SetGlobalVariable();
}

void WaveFloorChip::Initialize()
{
	wavePowers_.clear();
}

void WaveFloorChip::Update()
{
	float y = 0.0f;

	for (WavePower& wavePower : wavePowers_) {
		wavePower.time += frameInfo_->GetDeltaTime();
		if ((wavePower.time - frameInfo_->GetDeltaTime() < fParas_[kLoopTime] / 2 &&
			wavePower.time > fParas_[kLoopTime] / 2) ||
			(wavePower.time - frameInfo_->GetDeltaTime() < fParas_[kLoopTime] &&
				wavePower.time > fParas_[kLoopTime])) {
			wavePower.radius *= fParas_[kAttenuation];
			if (wavePower.radius <= fParas_[kMinSpeed]) {
				wavePower.time = 0.0f;
				wavePower.radius = 0.0f;
			}
			if (wavePower.time - frameInfo_->GetDeltaTime() < fParas_[kLoopTime] &&
				wavePower.time > fParas_[kLoopTime]) {
				wavePower.time -= fParas_[kLoopTime];
			}
		}
		float theta = 6.28f * wavePower.time / fParas_[kLoopTime] + 4.71f;

		y += wavePower.radius * std::sinf(theta);
	}

	for (WavePower& wavePower : wavePowers_) {
		wavePower.time += frameInfo_->GetDeltaTime();
		if ((wavePower.time - frameInfo_->GetDeltaTime() < fParas_[kLoopTime] / 2 &&
			wavePower.time > fParas_[kLoopTime] / 2) ||
			(wavePower.time - frameInfo_->GetDeltaTime() < fParas_[kLoopTime] &&
				wavePower.time > fParas_[kLoopTime])) {
			wavePower.radius *= fParas_[kAttenuation];
			if (wavePower.radius <= fParas_[kMinSpeed]) {
				wavePower.time = 0.0f;
				wavePower.radius = 0.0f;
			}
			if (wavePower.time - frameInfo_->GetDeltaTime() < fParas_[kLoopTime] &&
				wavePower.time > fParas_[kLoopTime]) {
				wavePower.time -= fParas_[kLoopTime];
			}
		}
		float theta = 6.28f * wavePower.time / fParas_[kLoopTime] + 4.71f;

		y += wavePower.radius * std::sinf(theta);
	}
	
	position_ = ganeratePosition_;
	position_.y += y;
}

void WaveFloorChip::Draw() const 
{
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(scale_, rotate_, position_);
	instancingManager_->AddBox(modelData_, InstancingModel{ matrix,{1.0f,1.0f,1.0f,1.0f} });
}

void WaveFloorChip::StaticUpdate()
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG
}

void WaveFloorChip::AddOutPower(float power)
{
	wavePowers_.push_back({ power,0.0f });
}

void WaveFloorChip::SetGlobalVariable()
{
	for (int i = 0; i < kFloatEnd; i++) {
		globalVariable_->AddItem(fNames[i], fParas_[i]);
	}

	ApplyGlobalVariable();
}

void WaveFloorChip::ApplyGlobalVariable()
{
	for (int i = 0; i < kFloatEnd; i++) {
		fParas_[i] = globalVariable_->GetFloatValue(fNames[i]);

		if (i == FloatParamater::kAttenuation) {
			fParas_[i] = std::clamp(fParas_[i], 0.0001f, 1.0f);
		}
		else {
			if (fParas_[i] < 0.0001f) {
				fParas_[i] = 0.0001f;
			}
		}
	}

	scale_ = { fParas_[kScale],fParas_[kScale] ,fParas_[kScale] };
}