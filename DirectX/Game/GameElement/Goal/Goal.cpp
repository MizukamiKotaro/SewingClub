#include "Goal.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "GameElement/Wave/Wave.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include "SceneSystem/IScene/IScene.h"
#include "GameElement/Animation/AnimationManager.h"

Goal::Goal()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("plane");
	const Texture* tex = TextureManager::GetInstance()->LoadTexture("goal_anime.png");
	modelData_ = instancingManager_->GetDrawData({ modelData,tex,BlendMode::kBlendModeNormal });

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

	seGoal_.LoadWave("SE/goal.wav");
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	// アニメーション
	animation_ = std::make_unique<Animation2D>(AnimationManager::GetInstance()->AddAnimation("goal"));
	// UV座標のセット
	Transform handle = animation_->GetSceneUV(0u);
	animation_->Play(true, false);
	
	edusts_ = std::make_unique<EffectGoalDusts>();
}

void Goal::Initialize()
{
	position_ = { 8.0f,1.0f,-0.02f };
	stageEditor_->Initialize();
	SetGlobalVariable();
	isHit_ = false;
	Transform handle = animation_->GetSceneUV(0u);
	animation_->Play(true, false);

	edusts_->Initialize(position_);
	edusts_->SetActive(true);
}

bool Goal::Update(float deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
	if (stageEditor_) {
		scale_ = maxScale_;
		if (stageEditor_->IsChangedStage()) {
			Initialize();
		}
	}
#endif // _DEBUG
	
	deltaTime = deltaTime;
	SetCollider();
	bool result = animation_->Update("goal");
	edusts_->Update();
	return result;
}

void Goal::Draw() const
{
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
	Matrix4x4 uvMatrix = animation_->GetUVTrans().worldMat_;
	instancingManager_->AddBox(modelData_, InstancingModelData{ matrix,uvMatrix, color_ });

	edusts_->Draw();
}

void Goal::SetGlobalVariable()
{
	if (stageEditor_) {
		stageEditor_->AddItem("ポジション", position_);
		stageEditor_->AddItem("スケール", maxScale_);
		stageEditor_->AddItem("カラー", Vector3{ 1.0f,1.0f,1.0f });
	}
	ApplyGlobalVariable();
}

void Goal::ApplyGlobalVariable()
{
	if (stageEditor_) {
		position_ = stageEditor_->GetVector3Value("ポジション");
		maxScale_ = stageEditor_->GetFloatValue("スケール");
		Vector3 color = stageEditor_->GetVector3Value("カラー");
		color_ = { color.x,color.y,color.z,1.0f };
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

void Goal::UpDownScale() {

}
