#include "WaterChunk.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "GameElement/Wave/Wave.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"

InstancingModelManager* WaterChunk::instancingManager_ = nullptr;
const ModelData* WaterChunk::modelData_ = nullptr;

std::unique_ptr<GlobalVariableUser> WaterChunk::staticGlobalVariable_ = nullptr;
float WaterChunk::deleteTime_ = 2.0f;

WaterChunk::WaterChunk()
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::WATER);
	Collider::AddTargetMask(ColliderMask::PLAYER);
	Collider::AddTargetMask(ColliderMask::CLIENT);

	gravityArea_ = std::make_unique<GravityArea>();

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
	maxScale_ = scale_;
	rotate_ = 0.0f;
	isSmaeGravitySize_ = false;
	no_ = 0;
	isSmall_ = false;
}

WaterChunk::WaterChunk(int no)
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::WATER);
	Collider::AddTargetMask(ColliderMask::PLAYER);

	gravityArea_ = std::make_unique<GravityArea>();

	position_ = {};
	scale_ = 1.0f;
	maxScale_ = scale_;
	rotate_ = 0.0f;

	no_ = no;
	isSmall_ = false;
	globalVariable_ = std::make_unique<GlobalVariableUser>("Water", "Water");
	SetGlobalVariable();
	scale_ = maxScale_;
	isSmaeGravitySize_ = false;
}

WaterChunk::WaterChunk(const Vector2& pos, const Vector2& radius, bool isSame, const float& rotate, bool isSmall)
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::WATER);
	Collider::AddTargetMask(ColliderMask::PLAYER);

	gravityArea_ = std::make_unique<GravityArea>();

	position_ = { pos.x,pos.y,0.0f };
	scale_ = radius.x;
	maxScale_ = scale_;
	rotate_ = rotate;
	isSmall_ = isSmall;
	time_ = 0.0f;

	isSmaeGravitySize_ = isSame;
}

void WaterChunk::StaticInitialize()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	modelData_ = ModelDataManager::GetInstance()->LoadObj("WaterCircle");

	StaticSetGlobalVariable();
}

void WaterChunk::Initialize()
{

}

void WaterChunk::Update(float deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG

	if (isSmall_) {
		time_ += deltaTime;

		time_ = std::clamp(time_, 0.0f, deleteTime_);
		scale_ = (1.0f - time_ / deleteTime_) * maxScale_;
	}
	/*for (std::unique_ptr<WaterChunkChip>& chip : chips_) {
		chip->Update();
	}*/
	gravityArea_->Update({ position_.x,position_.y }, { scale_,scale_ }, isSmaeGravitySize_, rotate_);
	SetCollider();
}

void WaterChunk::Draw() const
{
	/*for (std::unique_ptr<WaterChunkChip>& chip : chips_) {
		chip->Draw();
	}*/
#ifdef _DEBUG
	gravityArea_->Draw({ position_.x,position_.y }, { scale_,scale_ }, isSmaeGravitySize_, rotate_);
#endif // _DEBUG

	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
	instancingManager_->AddBox(modelData_, InstancingModel{ matrix,{0.3f,1.0f,0.8f,1.0f} });
}

void WaterChunk::StaticUpdate()
{
#ifdef _DEBUG
	StaticApplyGlobalVariable();
#endif // _DEBUG
}

void WaterChunk::SetGlobalVariable()
{
	if (globalVariable_) {
		std::string tree = "水の惑星" + std::to_string(no_);
		globalVariable_->AddItem("ポジション", position_, tree);
		globalVariable_->AddItem("スケール", maxScale_, tree);
	}
	ApplyGlobalVariable();
}

void WaterChunk::ApplyGlobalVariable()
{
	if (globalVariable_) {
		std::string tree = "水の惑星" + std::to_string(no_);
		position_ = globalVariable_->GetVector3Value("ポジション", tree);
		maxScale_ = globalVariable_->GetFloatValue("スケール", tree);
	}
}

void WaterChunk::StaticSetGlobalVariable()
{
	staticGlobalVariable_ = std::make_unique<GlobalVariableUser>("Charactor", "Player");
	staticGlobalVariable_->AddItem("水が消えるまでの時間", deleteTime_, "水の生成関係");
	StaticApplyGlobalVariable();
}

void WaterChunk::StaticApplyGlobalVariable()
{
	deleteTime_ = staticGlobalVariable_->GetFloatValue("水が消えるまでの時間", "水の生成関係");

	if (deleteTime_ <= 0.0f) {
		0.1f;
	}
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
