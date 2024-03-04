//#include "Player.h"
//#include <cassert>
//#include "Ease.h"
//#include "GlobalVariables.h"
//
//
//void Player::Initialize(const std::vector<Model*>& models) {
//	
//	BaseCharacter::Initialize(models);
//
//	worldTransform_.Initialize();
//	worldTransformBody_.Initialize();
//	worldTransformBody_.parent_ = &worldTransform_;
//	worldTransformHead_.Initialize();
//	worldTransformHead_.translation_.y = 3.0f;
//	worldTransformHead_.parent_ = &worldTransformBody_;
//	worldTransformL_arm_.Initialize();
//	worldTransformL_arm_.translation_ = {1.0f, 2.5f, 0.0f};
//	worldTransformL_arm_.parent_ = &worldTransformBody_;
//	worldTransformR_arm_.Initialize();
//	worldTransformR_arm_.translation_ = {-1.0f, 2.5f, 0.0f};
//	worldTransformR_arm_.parent_ = &worldTransformBody_;
//
//	worldTransformWeapon_.Initialize();
//	worldTransformWeapon_.parent_ = &worldTransformHead_;
//
//	InitializeFloatingGimmick();
//
//	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
//	const char* groupName = "Player";
//	GlobalVariables::GetInstance()->CreateGroup(groupName);
//
//	/*globalVariables->AddItem(groupName, "Test", 90);
//
//	float testf = 0.5f;
//	Vector3 testv = {1.0f, 0.0f, 3.3f};
//
//	globalVariables->AddItem(groupName, "TestFloat", testf);
//	globalVariables->AddItem(groupName, "TestVector3", testv);*/
//
//	globalVariables->AddItem(groupName, "Head Translation", worldTransformHead_.translation_);
//
//	
//}
//
//void Player::ApplyGlobalVariable() {
//
//
//	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
//	const char* groupName = "Player";
//
//	/*globalVariables->AddItem(groupName, "Test", 90);
//
//	float testf = 0.5f;
//	Vector3 testv = {1.0f, 0.0f, 3.3f};
//
//	globalVariables->AddItem(groupName, "TestFloat", testf);
//	globalVariables->AddItem(groupName, "TestVector3", testv);*/
//
//	worldTransformHead_.translation_ = globalVariables->GetVector3Value(groupName, "Head Translation");
//
//	
//}
//
//void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }
//
//void Player::UpdateFloatingGimmick() {
//
//	const uint16_t cycle = 100;
//
//	const float pi = 3.14f;
//
//	const float step = 2.0f * pi / cycle;
//
//	floatingParameter_ += step;
//
//	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * pi);
//	//浮遊の振幅
//	const float amplitude = 0.5f;
//
//	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitude;
//
//	const float angle = pi / 6;
//
//	worldTransformL_arm_.rotation_.z = std::sin(floatingParameter_) * angle;
//	worldTransformR_arm_.rotation_.z = std::sin(floatingParameter_) * angle;
//}
//
//void Player::BehaviorRootInitialize() { InitializeFloatingGimmick(); }
//
//void Player::BehaviorRootUpdate() {
//	BaseCharacter::Update();
//
//	UpdateFloatingGimmick();
//
//	// ゲームパッドの取得情報を得る関数
//	XINPUT_STATE joyState;
//
//	// ジョイスティック状態取得
//	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
//
//		const float speed = 2.0f;
//
//		Vector3 move = {
//		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
//		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
//
//		move = move.Normalize() * speed;
//
//		move = move * Matrix4x4::MakeRotateXYZMatrix(viewProjection_->rotation_);
//
//		worldTransform_.translation_ += move;
//
//		if (move.x == 0 && move.z == 0) {
//			// worldTransform_.rotation_.y = viewProjection_->rotation_.y;
//		} else {
//			worldTransform_.rotation_.y = atan2(move.x, move.z);
//		}
//	}
//
//	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
//		behaviorRequest_ = Behavior::kAttack;
//	}
//
//	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
//		GlobalVariables::GetInstance()->SaveFile("Player");
//	}
//}
//
//void Player::BehaviorAttackInitialize() { behaviorAttackRequest_ = BehaviorAttack::kExtra; }
//
//void Player::EaseVectorClear() { 
//	easeStartPos_.clear();
//	easeEndPos_.clear();
//	easeStartRot_.clear();
//	easeEndRot_.clear();
//}
//
//void Player::AttackBehaviorExtraInitialize() {
//
//	EaseVectorClear();
//
//	count_ = 0;
//
//	easeStartPos_.push_back(worldTransformBody_.translation_);
//	easeEndPos_.push_back({0.0f, 0.0f, 0.0f});
//	easeStartRot_.push_back(worldTransformBody_.rotation_);
//	easeEndRot_.push_back(worldTransformBody_.rotation_);
//
//	easeStartRot_.push_back(worldTransformL_arm_.rotation_);
//	easeEndRot_.push_back({-3.14f / 2.0f, 0.0f, 0.0f});
//
//	easeStartRot_.push_back(worldTransformR_arm_.rotation_);
//	easeEndRot_.push_back({-3.14f / 2.0f, 0.0f, 0.0f});
//}
//
//void Player::AttackBehaviorExtraUpdate() {
//
//	const int maxFrame = 5;
//	count_++;
//
//	worldTransformBody_.translation_ = Ease::UseEase(
//		easeStartPos_[kBody], easeEndPos_[kBody], count_, maxFrame, Ease::Constant, 2);
//	worldTransformL_arm_.rotation_ = Ease::UseEase(
//		easeStartRot_[kLArm], easeEndRot_[kLArm], count_, maxFrame, Ease::Constant, 2);
//	worldTransformR_arm_.rotation_ = Ease::UseEase(
//	    easeStartRot_[kRArm], easeEndRot_[kRArm], count_, maxFrame, Ease::Constant, 2);
//
//	if (count_ == maxFrame) {
//		behaviorAttackRequest_ = BehaviorAttack::kExtra2;
//		count_ = 0;
//	}
//}
//
//void Player::AttackBehaviorExtra2Initialize() {
//	EaseVectorClear();
//
//	count_ = 0;
//
//	easeStartRot_.push_back(worldTransformBody_.rotation_);
//	easeEndRot_.push_back(worldTransformBody_.rotation_);
//
//	easeStartRot_.push_back(worldTransformL_arm_.rotation_);
//	easeEndRot_.push_back({-3.14f / 6.0f - 3.14f, 0.0f, 0.0f});
//
//	easeStartRot_.push_back(worldTransformR_arm_.rotation_);
//	easeEndRot_.push_back({-3.14f / 6.0f - 3.14f, 0.0f, 0.0f});
//
//	easeStartRot_.push_back({3.14f / 2.0f, 0.0f, 0.0f});
//	easeEndRot_.push_back({-3.14f / 6.0f, 0.0f, 0.0f});
//}
//
//void Player::AttackBehaviorExtra2Update() {
//	const int maxFrame = 15;
//	count_++;
//
//	worldTransformL_arm_.rotation_ = Ease::UseEase(
//	    easeStartRot_[kLArm], easeEndRot_[kLArm], count_, maxFrame, Ease::Constant, 2);
//	worldTransformR_arm_.rotation_ = Ease::UseEase(
//	    easeStartRot_[kRArm], easeEndRot_[kRArm], count_, maxFrame, Ease::Constant, 2);
//	worldTransformWeapon_.rotation_ = Ease::UseEase(
//	    easeStartRot_[kWeapon], easeEndRot_[kWeapon], count_, maxFrame, Ease::Constant, 2);
//
//	if (count_ == maxFrame) {
//		behaviorAttackRequest_ = BehaviorAttack::kAttack;
//	}
//}
//
//void Player::AttackBehaviorAttackInitialize() { 
//	EaseVectorClear();
//
//	count_ = 0;
//
//	easeStartRot_.push_back(worldTransformBody_.rotation_);
//	easeEndRot_.push_back(worldTransformBody_.rotation_);
//
//	easeStartRot_.push_back(worldTransformL_arm_.rotation_);
//	easeEndRot_.push_back({-3.14f / 2.0f, 0.0f, 0.0f});
//
//	easeStartRot_.push_back(worldTransformR_arm_.rotation_);
//	easeEndRot_.push_back({-3.14f / 2.0f, 0.0f, 0.0f});
//
//	easeStartRot_.push_back(worldTransformWeapon_.rotation_);
//	easeEndRot_.push_back({3.14f / 2.0f, 0.0f, 0.0f});
//}
//
//void Player::AttackBehaviorAttackUpdate() {
//
//	const int maxFrame = 10;
//	count_++;
//
//	worldTransformL_arm_.rotation_ = Ease::UseEase(
//	    easeStartRot_[kLArm], easeEndRot_[kLArm], count_, maxFrame, Ease::Constant, 2);
//	worldTransformR_arm_.rotation_ = Ease::UseEase(
//	    easeStartRot_[kRArm], easeEndRot_[kRArm], count_, maxFrame, Ease::Constant, 2);
//	worldTransformWeapon_.rotation_ = Ease::UseEase(
//	    easeStartRot_[kWeapon], easeEndRot_[kWeapon], count_, maxFrame, Ease::Constant, 2);
//
//	if (count_ == maxFrame) {
//		behaviorAttackRequest_ = BehaviorAttack::kRigor;
//	}
//}
//
//void Player::AttackBehaviorRigorInitialize() { count_ = 0; }
//
//void Player::AttackBehaviorRigorUpdate() {
//
//	const int maxFrame = 20;
//	count_++;
//
//	if (count_ == maxFrame) {
//		behaviorAttackRequest_ = BehaviorAttack::kReturn;
//	}
//}
//
//void Player::AttackBehaviorReturnInitialize() {
//	EaseVectorClear();
//	count_ = 0;
//
//	easeStartRot_.push_back(worldTransformBody_.rotation_);
//	easeEndRot_.push_back(worldTransformBody_.rotation_);
//
//	easeStartRot_.push_back(worldTransformL_arm_.rotation_);
//	easeEndRot_.push_back({0.0f, 0.0f, 0.0f});
//
//	easeStartRot_.push_back(worldTransformR_arm_.rotation_);
//	easeEndRot_.push_back({0.0f, 0.0f, 0.0f});
//}
//
//void Player::AttackBehaviorReturnUpdate() {
//
//	const int maxFrame = 15;
//
//	count_++;
//
//	worldTransformL_arm_.rotation_ = Ease::UseEase(
//	    easeStartRot_[kLArm], easeEndRot_[kLArm], count_, maxFrame, Ease::Constant, 2);
//	worldTransformR_arm_.rotation_ = Ease::UseEase(
//	    easeStartRot_[kRArm], easeEndRot_[kRArm], count_, maxFrame, Ease::Constant, 2);
//
//	if (count_ == maxFrame) {
//		behaviorRequest_ = Behavior::kRoot;
//
//	}
//}
//
//void Player::BehaviorAttackUpdate() {
//
//	if (behaviorAttackRequest_) {
//		// 振る舞いを変更する
//		behaviorAttack_ = behaviorAttackRequest_.value();
//
//		// 各振る舞いごとの初期化を実行
//		switch (behaviorAttack_) {
//		case BehaviorAttack::kExtra:
//
//			AttackBehaviorExtraInitialize();
//			break;
//		case BehaviorAttack::kExtra2:
//
//			AttackBehaviorExtra2Initialize();
//			break;
//		case BehaviorAttack::kAttack:
//
//			AttackBehaviorAttackInitialize();
//			break;
//		case BehaviorAttack::kRigor:
//
//			AttackBehaviorRigorInitialize();
//			break;
//		case BehaviorAttack::kReturn:
//
//			AttackBehaviorReturnInitialize();
//			break;
//		default:
//			break;
//		}
//		// 振る舞いリクエストをリセット
//		behaviorAttackRequest_ = std::nullopt;
//	}
//
//	switch (behaviorAttack_) {
//	case BehaviorAttack::kExtra:
//
//		AttackBehaviorExtraUpdate();
//		break;
//	case BehaviorAttack::kExtra2:
//
//		AttackBehaviorExtra2Update();
//		break;
//	case BehaviorAttack::kAttack:
//
//		AttackBehaviorAttackUpdate();
//		break;
//	case BehaviorAttack::kRigor:
//
//		AttackBehaviorRigorUpdate();
//		break;
//	case BehaviorAttack::kReturn:
//
//		AttackBehaviorReturnUpdate();
//		break;
//	}
//}
//
//void Player::Update() {
//
//	ApplyGlobalVariable();
//
//	if (behaviorRequest_) {
//		// 振る舞いを変更する
//		behavior_ = behaviorRequest_.value();
//
//		// 各振る舞いごとの初期化を実行
//		switch (behavior_) {
//		case Behavior::kRoot:
//			BehaviorRootInitialize();
//			break;
//		case Behavior::kAttack:
//			BehaviorAttackInitialize();
//
//			break;
//		}
//		//振る舞いリクエストをリセット
//		behaviorRequest_ = std::nullopt;
//
//	}
//
//	switch (behavior_) {
//	case Behavior::kRoot:
//		BehaviorRootUpdate();
//
//		break;
//	case Behavior::kAttack:
//		BehaviorAttackUpdate();
//
//		break;
//	}
//
//	worldTransform_.UpdateMatrix();
//	worldTransformBody_.UpdateMatrix();
//	worldTransformHead_.UpdateMatrix();
//	worldTransformL_arm_.UpdateMatrix();
//	worldTransformR_arm_.UpdateMatrix();
//	worldTransformWeapon_.UpdateMatrix();
//}
//
//void Player::Draw(const ViewProjection& viewProjection) {
//	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
//	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
//	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
//	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
//
//	if (behavior_ == Behavior::kAttack) {
//		if (behaviorAttack_ != BehaviorAttack::kExtra &&
//		    behaviorAttack_ != BehaviorAttack::kReturn) {
//			models_[kModelIndexWeapon]->Draw(worldTransformWeapon_, viewProjection);
//		}
//	}
//}