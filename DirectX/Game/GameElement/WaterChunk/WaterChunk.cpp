#include "WaterChunk.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "GameElement/Wave/Wave.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include "SceneSystem/IScene/IScene.h"
#include "WindowsInfo/WindowsInfo.h"
#include "Camera.h"

InstancingModelManager* WaterChunk::instancingManager_ = nullptr;
const InstancingMeshTexData* WaterChunk::modelData_ = nullptr;

std::unique_ptr<GlobalVariableUser> WaterChunk::staticGlobalVariable_ = nullptr;
float WaterChunk::deleteTime_ = 2.0f;

WaterChunk::WaterChunk()
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::WATER);
	//Collider::AddTargetMask(ColliderMask::PLAYER);
	//Collider::AddTargetMask(ColliderMask::CLIENT);

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
	isTree_ = false;
	color_ = { 0.3f,1.0f,0.8f,1.0f };
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
	stageEditor_ = std::make_unique<StageEditor>("水の配置");
	SetGlobalVariable();
	scale_ = maxScale_;
	isSmaeGravitySize_ = false;
	isTree_ = false;
	color_ = { 0.3f,1.0f,0.8f,1.0f };
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
	isTree_ = false;
	color_ = { 0.3f,1.0f,0.8f,1.0f };
}

void WaterChunk::StaticInitialize()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("WaterCircle");
	modelData_ = instancingManager_->GetDrawData({ modelData,modelData->texture,BlendMode::kBlendModeNormal });

	StaticSetGlobalVariable();
}

void WaterChunk::Initialize()
{

}

void WaterChunk::Update(float deltaTime, Camera* camera)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
	isTree_ = false;
	if (stageEditor_) {
		scale_ = maxScale_;
		std::string tree = "水" + std::to_string(no_);
		int no = no_ / 10;
		no = no * 10;
		std::string tree1 = "水" + std::to_string(no) + "～" + std::to_string(no + 9);
		if (stageEditor_->IsTreeOpen(tree1, tree)) {
			color_ = { 1.0f,0.3f,0.3f,1.0f };
		}
		else if (stageEditor_->IsTreeOpen(tree1)) {
			color_ = { 0.8f,0.7f,0.1f,1.0f };
		}
		else {
			color_ = { 0.3f,1.0f,0.8f,1.0f };
		}
	}
#endif // _DEBUG

	if (isSmall_) {
		time_ += deltaTime;

		time_ = std::clamp(time_, 0.0f, deleteTime_);
		scale_ = (1.0f - time_ / deleteTime_) * maxScale_;
	}

	ActiveCheck(camera);
	/*for (std::unique_ptr<WaterChunkChip>& chip : chips_) {
		chip->Update();
	}*/
	if (isActive_) {
		gravityArea_->Update({ position_.x,position_.y }, { scale_,scale_ }, isSmaeGravitySize_, rotate_);
		SetCollider();
	}
}

void WaterChunk::Draw() const
{
	if (isActive_) {
		/*for (std::unique_ptr<WaterChunkChip>& chip : chips_) {
		chip->Draw();
	}*/
#ifdef _DEBUG
		gravityArea_->Draw({ position_.x,position_.y }, { scale_,scale_ }, isSmaeGravitySize_, rotate_);
#endif // _DEBUG
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
		instancingManager_->AddBox(modelData_, InstancingModelData{ matrix, Matrix4x4::MakeIdentity4x4(), color_ });
	}
}

void WaterChunk::StaticUpdate()
{
#ifdef _DEBUG
	StaticApplyGlobalVariable();
#endif // _DEBUG
}

void WaterChunk::SetGlobalVariable()
{
	if (stageEditor_) {
		std::string tree = "水" + std::to_string(no_);
		int no = no_ / 10;
		no = no * 10;
		std::string tree1 = "水" + std::to_string(no) + "～" + std::to_string(no + 9);
		stageEditor_->AddItem("ポジション", position_, tree1, tree);
		stageEditor_->AddItem("スケール", maxScale_, tree1, tree);
	}
	ApplyGlobalVariable();
}

void WaterChunk::ApplyGlobalVariable()
{
	if (stageEditor_) {
		std::string tree = "水" + std::to_string(no_);
		int no = no_ / 10;
		no = no * 10;
		std::string tree1 = "水" + std::to_string(no) + "～" + std::to_string(no + 9);
		position_ = stageEditor_->GetVector3Value("ポジション", tree1, tree);
		maxScale_ = stageEditor_->GetFloatValue("スケール", tree1, tree);
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

void WaterChunk::ActiveCheck(Camera* camera)
{
	Vector2 win = WindowsInfo::GetInstance()->GetWindowSize();
	float ratio = win.y / (std::tanf(0.225f) * (position_.z - camera->transform_.translate_.z) * 2);

	Vector2 pos{};
	pos.x = position_.x * ratio - camera->transform_.translate_.x * ratio + camera->transform_.translate_.x;
	pos.y = position_.y * ratio - camera->transform_.translate_.y * ratio + camera->transform_.translate_.y;

	float radius = scale_ * ratio;

	if ((std::abs(pos.x) - radius > win.x * 0.7f) || (std::abs(pos.y) - radius > win.y * 0.7f)) {
		isActive_ = false;
	}
	else {
		isActive_ = true;
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
