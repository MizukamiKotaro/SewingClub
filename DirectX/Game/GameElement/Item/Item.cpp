#include "Item.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "GameElement/Wave/Wave.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include "SceneSystem/IScene/IScene.h"

InstancingModelManager* Item::instancingManager_ = nullptr;
const ModelData* Item::modelData_ = nullptr;

std::unique_ptr<GlobalVariableUser> Item::staticGlobalVariable_ = nullptr;
float Item::deleteTime_ = 2.0f;

Item::Item(int no)
{
	isHit_ = false;
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::ITEM);
	Collider::AddTargetMask(ColliderMask::PLAYER);

	position_ = { 0.0f,1.0f,-0.02f };
	scale_ = 0.5f;
	maxScale_ = scale_;
	rotate_ = 0.0f;

	no_ = no;
	isSmall_ = false;
	stageEditor_ = std::make_unique<StageEditor>("アイテムの配置");
	SetGlobalVariable();
	scale_ = maxScale_;
	color_ = { 1.0f,1.0f,0.3f,1.0f };
}

void Item::StaticInitialize()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	modelData_ = ModelDataManager::GetInstance()->LoadObj("WaterCircle");

	StaticSetGlobalVariable();
}

void Item::Initialize()
{

}

void Item::Update(float deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
	if (stageEditor_) {
		scale_ = maxScale_;
		std::string tree = "アイテム" + std::to_string(no_);
		int no = no_ / 10;
		no = no * 10;
		std::string tree1 = "アイテム" + std::to_string(no) + "～" + std::to_string(no + 9);
		if (stageEditor_->IsTreeOpen(tree1, tree)) {
			color_ = { 1.0f,0.3f,0.3f,1.0f };
		}
		else if (stageEditor_->IsTreeOpen(tree1)) {
			color_ = { 0.7f,0.6f,0.1f,1.0f };
		}
		else {
			color_ = { 1.0f,1.0f,0.3f,1.0f };
		}
	}
#endif // _DEBUG

	/*if (isSmall_) {
		time_ += deltaTime;

		time_ = std::clamp(time_, 0.0f, deleteTime_);
		scale_ = (1.0f - time_ / deleteTime_) * maxScale_;
	}*/
	deltaTime = deltaTime;

	SetCollider();
}

void Item::Draw() const
{
	if (!isHit_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
		instancingManager_->AddBox(modelData_, InstancingModel{ matrix, color_ });
	}
}

void Item::StaticUpdate()
{
#ifdef _DEBUG
	StaticApplyGlobalVariable();
#endif // _DEBUG
}

void Item::SetGlobalVariable()
{
	if (stageEditor_) {
		std::string tree = "アイテム" + std::to_string(no_);
		int no = no_ / 10;
		no = no * 10;
		std::string tree1 = "アイテム" + std::to_string(no) + "～" + std::to_string(no + 9);
		stageEditor_->AddItem("ポジション", position_, tree1, tree);
		stageEditor_->AddItem("スケール", maxScale_, tree1, tree);
	}
	ApplyGlobalVariable();
}

void Item::ApplyGlobalVariable()
{
	if (stageEditor_) {
		std::string tree = "アイテム" + std::to_string(no_);
		int no = no_ / 10;
		no = no * 10;
		std::string tree1 = "アイテム" + std::to_string(no) + "～" + std::to_string(no + 9);
		position_ = stageEditor_->GetVector3Value("ポジション", tree1, tree);
		maxScale_ = stageEditor_->GetFloatValue("スケール", tree1, tree);
	}
}

void Item::StaticSetGlobalVariable()
{
	/*staticGlobalVariable_ = std::make_unique<GlobalVariableUser>("Charactor", "Player");
	staticGlobalVariable_->AddItem("水が消えるまでの時間", deleteTime_, "水の生成関係");
	StaticApplyGlobalVariable();*/
}

void Item::StaticApplyGlobalVariable()
{
	/*deleteTime_ = staticGlobalVariable_->GetFloatValue("水が消えるまでの時間", "水の生成関係");

	if (deleteTime_ <= 0.0f) {
		0.1f;
	}*/
}

void Item::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::PLAYER) {
		isHit_ = true;
	}
}

void Item::SetCollider()
{
	if (!isHit_) {
		Collider::SetCircle({ position_.x,position_.y }, scale_);
		collisionManager_->SetCollider(this);
	}
}