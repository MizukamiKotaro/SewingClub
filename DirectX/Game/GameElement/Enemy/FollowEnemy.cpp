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
	position_ = initialPosition_;
}

void FollowEnemy::StaticInitialize() {
	modelData_ = CreateData("enemy_undead.png");
}

void FollowEnemy::SetPlayerPtr(const Vector3* pplayer) {
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
		bool flag = ChackDistance();
		Move(flag);
		SetCollider();
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
	stageEditor_->AddItem("初期座標", initialPosition_);
	stageEditor_->AddItem("スケール", scale_);
	stageEditor_->AddItem("感知距離", kMaxDistance);
	stageEditor_->AddItem("速度", kMaxDistance);
	stageEditor_->AddItem("初期位置から一定範囲で追従する", isAreaMove_);
	stageEditor_->AddItem("現在位置から一定範囲で追従する", isAutoMove_);
	stageEditor_->AddItem("初期位置に戻るか", isReturn_);

	ApplyGlobalVariable();
}

void FollowEnemy::ApplyGlobalVariable() {
	initialPosition_ = stageEditor_->GetVector3Value("初期座標");
	scale_ = stageEditor_->GetFloatValue("スケール");
	kMaxDistance = stageEditor_->GetFloatValue("感知距離");
	speed_ = stageEditor_->GetFloatValue("速度");
	isAreaMove_ = stageEditor_->GetBoolValue("初期位置から一定範囲で追従する");
	isAutoMove_ = stageEditor_->GetBoolValue("現在位置から一定範囲で追従する");
	isReturn_ = stageEditor_->GetBoolValue("初期位置に戻るか");

	// debug用処理
	// 両方が同じにされた場合。現在位置から追従を優先する
	if (isAreaMove_ == isAutoMove_) {
		isAutoMove_ = true;
		isAreaMove_ = false;
	}

}

bool FollowEnemy::ChackDistance() const {
	if (player_ptr) {
		float length = 0.0f;
		if (isAreaMove_) {
			length = player_ptr->Length(initialPosition_);
		}
		else if (isAutoMove_) {
			length = player_ptr->Length(position_);
		}
		if (length < kMaxDistance) {
			// 感知距離内であれば
			return true;
		}
	}
	return false;
}

void FollowEnemy::Move(bool isFollowing) {
	// 敵からplayerのベクトルを求める
	Vector3 vec{};
	if (isFollowing) {
		// 追従時
		vec = *player_ptr - position_;
	}
	else if (isReturn_){
		// それ以外なら、初期位置に戻る
		if (initialPosition_.Length(position_) < speed_) {
			// 特例処理
			vec = Vector3(0.0f, 0.0f, 0.0f);
			position_ = initialPosition_;
		}
		else {
			vec = initialPosition_ - position_;
		}
	}
	// 純粋に足す
	position_ += vec.Normalize() * speed_;
}