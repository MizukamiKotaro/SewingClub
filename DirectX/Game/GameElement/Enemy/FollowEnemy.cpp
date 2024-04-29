#include "FollowEnemy.h"

const ParticleMeshTexData* FollowEnemy::modelData_ = nullptr;
const Vector3* FollowEnemy::player_ptr = nullptr;

FollowEnemy::FollowEnemy(const int& no) {
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::ENEMY);
	isActive_ = true;
	position_ = { 2.0f,2.0f,-0.3f };
	scale_ = 0.5f;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	CreateStageEditor("追従する敵", no);
	SetGlobalVariable();
}

void FollowEnemy::StaticInitialize(const Vector3* pplayer) {
	modelData_ = CreateData("enemy_undead.png");
	if (pplayer) {
		player_ptr = pplayer;
	}
}

void FollowEnemy::Initialize() {

}

void FollowEnemy::Update(const float& deltaTime, Camera* camera) {
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

	if (deltaTime) {

	}
	// カメラ内か
	if (isActive_) {
		// 感知距離内か
		if (ChackDistance()) {
			// 移動処理後にコライダー更新
			Move();
			SetCollider();
		}
	}
}

void FollowEnemy::Draw() const {
	if (isActive_) {
		DrawDefault(modelData_);
	}
}

void FollowEnemy::OnCollision(const Collider& collider) {
	// playerと衝突時
	if (collider.GetMask() == ColliderMask::PLAYER) {
		// playerの方で処理が書かれている
	}
}

void FollowEnemy::SetGlobalVariable() {
	stageEditor_->AddItem("初期座標", position_);
	stageEditor_->AddItem("スケール", scale_);
	stageEditor_->AddItem("感知距離", kMaxDistance);

	ApplyGlobalVariable();
}

void FollowEnemy::ApplyGlobalVariable() {
	position_ = stageEditor_->GetVector3Value("初期座標");
	scale_ = stageEditor_->GetFloatValue("スケール");
	kMaxDistance = stageEditor_->GetFloatValue("感知距離");
}

bool FollowEnemy::ChackDistance() const {
	if (player_ptr) {
		float length = player_ptr->Length(position_);
		if (length < kMaxDistance) {
			// 感知距離内であれば
			return true;
		}
	}
	return false;
}

void FollowEnemy::Move() {

}
