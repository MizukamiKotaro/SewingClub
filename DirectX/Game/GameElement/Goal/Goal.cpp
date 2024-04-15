#include "Goal.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "GameElement/Wave/Wave.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include "SceneSystem/IScene/IScene.h"

Goal::Goal()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	modelData_ = ModelDataManager::GetInstance()->LoadObj("WaterCircle");

	isHit_ = false;
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::GOAL);
	Collider::AddTargetMask(ColliderMask::PLAYER);

	position_ = { 8.0f,1.0f,-0.02f };
	scale_ = 1.5f;
	maxScale_ = scale_;
	rotate_ = 0.0f;

	isSmall_ = false;
	stageEditor_ = std::make_unique<StageEditor>("ゴールの配置");
	SetGlobalVariable();
	scale_ = maxScale_;
	color_ = { 0.2f,0.3f,0.3f,1.0f };

	seGoal_.LoadWave("SE/goal.wav");
}

void Goal::Initialize()
{
	position_ = { 8.0f,1.0f,-0.02f };
	stageEditor_->Initialize();
	SetGlobalVariable();
	isHit_ = false;
}

void Goal::Update(float deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
	if (stageEditor_) {
		scale_ = maxScale_;
		if (stageEditor_->IsTreeOpen()) {
			color_ = { 1.0f,0.3f,0.3f,1.0f };
		}else {
			color_ = { 0.2f,0.3f,0.3f,1.0f };
		}
		if (stageEditor_->IsChangedStage()) {
			Initialize();
		}
	}
#endif // _DEBUG
	
	deltaTime = deltaTime;

	SetCollider();
}

void Goal::Draw() const
{
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
	instancingManager_->AddBox(modelData_, InstancingModel{ matrix, color_ });
}

void Goal::SetGlobalVariable()
{
	if (stageEditor_) {
		stageEditor_->AddItem("ポジション", position_);
		stageEditor_->AddItem("スケール", maxScale_);
	}
	ApplyGlobalVariable();
}

void Goal::ApplyGlobalVariable()
{
	if (stageEditor_) {
		position_ = stageEditor_->GetVector3Value("ポジション");
		maxScale_ = stageEditor_->GetFloatValue("スケール");
	}
}

void Goal::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::PLAYER) {
		isHit_ = true;
		seGoal_.Play();
	}
}

void Goal::SetCollider()
{
	if (!isHit_) {
		Collider::SetCircle({ position_.x,position_.y }, scale_);
		collisionManager_->SetCollider(this);
	}
}