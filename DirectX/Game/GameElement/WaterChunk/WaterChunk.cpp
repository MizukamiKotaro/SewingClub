#include "WaterChunk.h"
#include "Camera.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "GameElement/Wave/Wave.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"

InstancingModelManager* WaterChunk::instancingManager_ = nullptr;
const ModelData* WaterChunk::modelData_ = nullptr;

WaterChunk::WaterChunk()
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::WATER);
	Collider::AddTargetMask(ColliderMask::PLAYER);

	/*float scale = WaterChunkChip::GetScale();
	int map = 100;
	for (int i = 0; i < map; i++) {
		float x = scale * i - scale * map / 2 - scale / 2;
		for (int j = 0; j < map; j++) {
			float z = scale * j - scale * map / 2 - scale / 2;
			chips_.push_back(std::make_unique<WaterChunkChip>(Vector3{ x,0.0f,z }));
		}
	}*/
	position_ = {};
	scale_ = 1.0f;
	rotate_ = 0.0f;
}

void WaterChunk::StaticInitialize()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	modelData_ = ModelDataManager::GetInstance()->LoadObj("WaterCircle");

	/*globalVariable_ = std::make_unique<GlobalVariableUser>("WaterChunk", "Chip");
	globalVariable_->CreateGroup();

	SetGlobalVariable();*/
}

void WaterChunk::Initialize()
{

}

void WaterChunk::Update()
{
	/*for (std::unique_ptr<WaterChunkChip>& chip : chips_) {
		chip->Update();
	}*/
	SetCollider();
}

void WaterChunk::Draw() const
{
	/*for (std::unique_ptr<WaterChunkChip>& chip : chips_) {
		chip->Draw();
	}*/
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
	instancingManager_->AddBox(modelData_, InstancingModel{ matrix,{0.3f,1.0f,0.8f,1.0f} });
}

void WaterChunk::StaticUpdate()
{
#ifdef _DEBUG
	//ApplyGlobalVariable();
#endif // _DEBUG
}

void WaterChunk::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::PLAYER) {
		
	}
}

void WaterChunk::SetCollider()
{
	Collider::SetCircle({ position_.x,position_.y }, scale_);
	collisionManager_->SetCollider(this);
}

//void WaterChunk::HitTest(const Wave& wave)
//{
//	/*for (std::unique_ptr<WaterChunkChip>& chip : chips_) {
//		const Vector3 pos = chip->GetPosition();
//		if (IsHitCircle(pos, wave.position_, wave.radius_) && !IsHitCircle(pos, wave.position_, wave.preRadius_)) {
//			float t = (pos - wave.position_).Length() / wave.maxRadius_;
//			chip->AddOutPower(wave.power_ * (1.0f - t));
//		}
//	}*/
//}

//void WaterChunk::SetGlobalVariable()
//{
//	for (int i = 0; i < kFloatEnd; i++) {
//		globalVariable_->AddItem(fNames[i], fParas_[i]);
//	}
//
//	ApplyGlobalVariable();
//}

//void WaterChunk::ApplyGlobalVariable()
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
