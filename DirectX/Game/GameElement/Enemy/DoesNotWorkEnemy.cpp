#include "DoesNotWorkEnemy.h"

const ParticleMeshTexData* DoesNotWorkEnemy::modelData_ = nullptr;

DoesNotWorkEnemy::DoesNotWorkEnemy(const int& no)
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::ENEMY);
	isActive_ = true;
	position_ = { 2.0f,2.0f,-0.3f };
	scale_ = 0.5f;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	CreateStageEditor("動かない敵", no);
	SetGlobalVariable();
}

void DoesNotWorkEnemy::StaticInitialize()
{
	modelData_ = CreateData("enemy_undead.png");
}

void DoesNotWorkEnemy::Initialize()
{
}

void DoesNotWorkEnemy::Update(const float& deltaTime, Camera* camera, const uint32_t& babyTension)
{
#ifdef _DEBUG
	ApplyGlobalVariable();

	if (stageEditor_->IsTreeOpen(3)) {
		color_ = { 1.0f,0.0f,0.0f,1.0f };
	}
	else if (stageEditor_->IsTreeOpen(2)) {
		color_ = { 0.8f,0.2f,0.8f,1.0f };
	}
	else if (stageEditor_->IsTreeOpen(1)) {
		color_ = { 0.0f,1.0f,1.0f,1.0f };
	}
	else {
		color_ = { 1.0f,1.0f,1.0f,1.0f };
	}
#endif // _DEBUG

	isActive_ = camera->InScreenCheck2D(position_, scale_);

	if (deltaTime || babyTension) {

	}
	if (isActive_) {
		SetCollider();
	}
}

void DoesNotWorkEnemy::Draw() const
{
	if (isActive_) {
		DrawDefault(modelData_);
	}
}

void DoesNotWorkEnemy::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::PLAYER) {

	}
}

void DoesNotWorkEnemy::SetGlobalVariable()
{
	stageEditor_->AddItem("初期座標", position_);
	stageEditor_->AddItem("スケール", scale_);

	ApplyGlobalVariable();
}

void DoesNotWorkEnemy::ApplyGlobalVariable()
{
	position_ = stageEditor_->GetVector3Value("初期座標");
	scale_ = stageEditor_->GetFloatValue("スケール");
}
