#include "WaveFloorChip.h"
#include "Camera.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "FrameInfo/FrameInfo.h"

InstancingModelManager* WaveFloorChip::instancingManager_ = nullptr;
const InstancingMeshTexData* WaveFloorChip::modelData_ = nullptr;
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
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("Cube");
	modelData_ = instancingManager_->GetDrawData({ modelData,modelData->texture,BlendMode::kBlendModeNormal });
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

	for (std::list<WavePower>::iterator it = wavePowers_.begin(); it != wavePowers_.end();) {
		(*it).time += frameInfo_->GetDeltaTime();
		if (((*it).time - frameInfo_->GetDeltaTime() < fParas_[kLoopTime] / 2 &&
			(*it).time > fParas_[kLoopTime] / 2) ||
			((*it).time - frameInfo_->GetDeltaTime() < fParas_[kLoopTime] &&
				(*it).time > fParas_[kLoopTime])) {
			(*it).radius *= fParas_[kAttenuation];
			if ((*it).radius <= fParas_[kMinSpeed]) {
				(*it).time = 0.0f;
				(*it).radius = 0.0f;
			}
			if ((*it).time - frameInfo_->GetDeltaTime() < fParas_[kLoopTime] &&
				(*it).time > fParas_[kLoopTime]) {
				(*it).time -= fParas_[kLoopTime];
			}
		}
		float theta = 6.28f * (*it).time / fParas_[kLoopTime] + 4.71f;

		y += (*it).radius * std::sinf(theta);

		if ((*it).radius == 0.0f) {
			it = wavePowers_.erase(it);
		}
		else {
			it++;
		}
	}
	
	position_ = ganeratePosition_;
	position_.y += y;
}

void WaveFloorChip::Draw() const 
{
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(scale_, rotate_, position_);
	instancingManager_->AddBox(modelData_, InstancingModelData{ matrix,Matrix4x4::MakeIdentity4x4() ,{1.0f,1.0f,1.0f,1.0f} });
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