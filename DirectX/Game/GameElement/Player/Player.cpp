#include "Player.h"
#include "Camera.h"
#include "Input.h"
#include "calc.h"
#include <algorithm>
#include "CollisionSystem/CollisionManager/CollisionManager.h"

Player::Player()
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::RIGID_BODY, ColliderMask::PLAYER);
	Collider::AddTargetMask(ColliderMask::WATER);

	CreateGlobalVariable("Player");

	input_ = Input::GetInstance();

	for (int i = 0; i < kFloatEnd; i++) {
		fParas_[i] = 0.5f;
	}

	velocity_ = {};
	vector_ = { 1.0f,0.0f };
	speed_ = 0.0f;
	addAcceleration_ = 0.0f;
	timeCount_ = 0.0f;

	model_ = std::make_unique<Model>("Cube");
	model_->transform_.scale_ = { 0.4f,0.2f,0.2f };
	SetGlobalVariable();
	model_->Update();

	floatingParameter_ = 0.0f;
	isInWater_ = true;
	preIsInWater_ = true;

	yarn_ = std::make_unique<Yarn>(&model_->transform_.translate_, model_->transform_.translate_);
}

void Player::Initialize()
{
	Reset();
}

void Player::Update(float deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG

	if (isInWater_ && preIsInWater_) {
		Move(deltaTime);
	}
	else if (!isInWater_ && preIsInWater_) {
		PopUpFromWater();
	}
	else if (isInWater_ && !preIsInWater_) {
		ComeToWater();
	}
	else {
		OutWater(deltaTime);
	}
	preIsInWater_ = isInWater_;

	if (model_->transform_.GetWorldPosition().y <= fParas_[kMinPositionY]) {
		Reset();
	}

	model_->Update();

	yarn_->Update();

	isInWater_ = false;
	SetCollider();
}

void Player::Draw(const Camera* camera)
{
	model_->Draw(*camera);

	//yarn_->Draw();
}

const Vector3& Player::GetPosition() const
{
	return model_->transform_.GetWorldPosition();
}

void Player::Move(float deltaTime)
{
	timeCount_ += deltaTime;

	Vector2 vector = input_->GetGamePadLStick();

	if (vector.x != 0.0f || vector.y != 0.0f) {
		vector = vector.Normalize();

		if (vector_.x != -vector.x) {
			vector_ = Calc::Lerp(vector_, vector, fParas_[kInterpolationRate]).Normalize();
		}
		else {
			// 真逆の時の細かい修正保留
			vector_ = Calc::Lerp(vector_, vector, fParas_[kInterpolationRate]).Normalize();
		}

		speed_ = std::clamp(speed_ + fParas_[kAcceleration] * deltaTime, fParas_[kMinSpeed] * deltaTime, fParas_[kMaxSpeed] * deltaTime + addAcceleration_);

		velocity_ = { vector_.x * speed_,vector_.y * speed_,0.0f };

		if (vector_.y >= 0.0f) {
			model_->transform_.rotate_.z = std::acosf(vector_.x);
		}
		else {
			model_->transform_.rotate_.z = -std::acosf(vector_.x);
		}
	}
	else {
		if (fParas_[kAttenuation] != 0.0f) {
			speed_ = speed_ * fParas_[kAttenuation];
		}

		if (speed_ >= fParas_[kMinSpeed] * deltaTime) {
			velocity_ = { vector_.x * speed_,vector_.y * speed_,0.0f };
		}
		else {
			velocity_ = {};
			speed_ = 0.0f;
		}
	}

	if (timeCount_ >= 0.1f) {
		addAcceleration_ *= 0.90f;
		if (addAcceleration_ <= 0.3f) {
			addAcceleration_ = 0.0f;
		}
	}

	model_->transform_.translate_ += velocity_;
}

void Player::PopUpFromWater()
{
	timeCount_ = 0.0f;
	model_->transform_.translate_ += velocity_;
}

void Player::ComeToWater()
{
	timeCount_ = 0.0f;
	model_->transform_.translate_ += velocity_;
}

void Player::OutWater(float deltaTime)
{
	timeCount_ += deltaTime;

	if (timeCount_ <= fParas_[kOutWaterTime]) {
		float addSpeed = fParas_[kOutWaterAcceleration] * deltaTime;
		speed_ += addSpeed;
		addAcceleration_ += addSpeed;
	}
	velocity_ = { vector_.x * speed_,vector_.y * speed_ ,0.0f };
	if (velocity_.y <= 0.0f) {
		velocity_.y -= fParas_[kGravityDown] * deltaTime;
	}
	else {
		velocity_.y -= fParas_[kGravity] * deltaTime;
	}

	Vector3 normal = velocity_.Normalize();
	vector_ = { normal.x,normal.y };

	if (std::abs(normal.x) > std::abs(normal.y)) {
		speed_ = velocity_.x / normal.x;
	}
	else {
		speed_ = velocity_.y / normal.y;
	}
	if (vector_.y >= 0.0f) {
		model_->transform_.rotate_.z = std::acosf(vector_.x);
	}
	else {
		model_->transform_.rotate_.z = -std::acosf(vector_.x);
	}

	model_->transform_.translate_ += velocity_;
}

void Player::Reset()
{
	velocity_ = { 0.0f };
	vector_ = { 1.0f,0.0f };
	speed_ = 0.0f;
	addAcceleration_ = 0.0f;
	timeCount_ = 0.0f;

	model_->transform_.rotate_ = { 0.0f };
	model_->transform_.translate_ = { 0.0f };

	floatingParameter_ = 0.0f;
	isInWater_ = true;
	preIsInWater_ = true;

	yarn_.reset(new Yarn(&model_->transform_.translate_, model_->transform_.translate_));
}

void Player::InitializeFloating()
{
	floatingParameter_ = 0.0f;
}

void Player::UpdateFloating()
{

}

void Player::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::WATER) {
		isInWater_ = true;
	}
}

void Player::SetCollider()
{
	Collider::SetCircle({ model_->transform_.translate_.x,model_->transform_.translate_.y },
		0.0f, 0.0f, { velocity_.x,velocity_.y });
	collisionManager_->SetCollider(this);
}

void Player::SetGlobalVariable()
{
	globalVariable_->AddItem("スケール", model_->transform_.scale_);

	for (int i = 0; i < kFloatEnd; i++) {
		globalVariable_->AddItem(fNames[i], fParas_[i]);
	}

	ApplyGlobalVariable();
}

void Player::ApplyGlobalVariable()
{
	model_->transform_.scale_ = globalVariable_->GetVector3Value("スケール");

	for (int i = 0; i < kFloatEnd; i++) {
		fParas_[i] = globalVariable_->GetFloatValue(fNames[i]);
	}
}