#include "WaveFloor.h"
#include "Camera.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "GameElement/Wave/Wave.h"

//InstancingModelManager* WaveFloor::instancingManager_ = nullptr;
//const ModelData* WaveFloor::modelData_ = nullptr;
//
//std::unique_ptr<GlobalVariableUser> WaveFloor::globalVariable_ = nullptr;
//
//const std::string WaveFloor::fNames[kFloatEnd] = {
//	"減速率",
//	"最低速度",
//	"間の最大の長さ",
//	"スケール"
//};
//
//float WaveFloor::fParas_[kFloatEnd] = { 1.0f };
//
//Vector3 WaveFloor::scale_ = { WaveFloor::fParas_[WaveFloor::kScale],WaveFloor::fParas_[WaveFloor::kScale] ,WaveFloor::fParas_[WaveFloor::kScale] };

WaveFloor::WaveFloor()
{
	float scale = WaveFloorChip::GetScale();
	int map = 100;
	for (int i = 0; i < map; i++) {
		float x = scale * i - scale * map / 2 - scale / 2;
		for (int j = 0; j < map; j++) {
			float z = scale * j - scale * map / 2 - scale / 2;
			chips_.push_back(std::make_unique<WaveFloorChip>(Vector3{ x,0.0f,z }));
		}
	}
}

void WaveFloor::StaticInitialize()
{
	/*instancingManager_ = InstancingModelManager::GetInstance();
	modelData_ = ModelDataManager::GetInstance()->LoadObj("Cube");

	globalVariable_ = std::make_unique<GlobalVariableUser>("WaveFloor", "Chip");
	globalVariable_->CreateGroup();

	SetGlobalVariable();*/
}

void WaveFloor::Initialize()
{

}

void WaveFloor::Update()
{
	for (std::unique_ptr<WaveFloorChip>& chip : chips_) {
		chip->Update();
	}
}

void WaveFloor::Draw()
{
	for (std::unique_ptr<WaveFloorChip>& chip : chips_) {
		chip->Draw();
	}
}

void WaveFloor::StaticUpdate()
{
#ifdef _DEBUG
	//ApplyGlobalVariable();
#endif // _DEBUG
}

void WaveFloor::HitTest(const Wave& wave)
{
	for (std::unique_ptr<WaveFloorChip>& chip : chips_) {
		const Vector3 pos = chip->GetPosition();
		if (IsHitCircle(pos, wave.position_, wave.radius_) && !IsHitCircle(pos, wave.position_, wave.preRadius_)) {
			float t = (pos - wave.position_).Length() / wave.maxRadius_;
			chip->AddOutPower(wave.power_ * (1.0f - t));
		}
	}
}

//void WaveFloor::SetGlobalVariable()
//{
//	for (int i = 0; i < kFloatEnd; i++) {
//		globalVariable_->AddItem(fNames[i], fParas_[i]);
//	}
//
//	ApplyGlobalVariable();
//}

//void WaveFloor::ApplyGlobalVariable()
//{
//	for (int i = 0; i < kFloatEnd; i++) {
//		fParas_[i] = globalVariable_->GetFloatValue(fNames[i]);
//
//		if (i == FloatParamater::kAttenuation) {
//			fParas_[i] = std::clamp(fParas_[i], 0.0001f, 1.0f);
//		}
//		else {
//			if (fParas_[i] < 0.0001f) {
//				fParas_[i] = 0.0001f;
//			}
//		}
//	}
//
//	scale_ = { fParas_[kScale],fParas_[kScale] ,fParas_[kScale] };
//}

bool WaveFloor::IsHitCircle(const Vector3& pos0, const Vector3& pos1, float radius)
{
	if ((pos0 - pos1).Length() <= radius) {
		return true;
	}
	return false;
}
