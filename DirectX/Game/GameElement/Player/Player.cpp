#include "Player.h"
#include "Camera.h"
#include "Input.h"
#include "calc.h"
#include <algorithm>

Player::Player()
{;
	CreateGlobalVariable("Player");

	input_ = Input::GetInstance();

	for (int i = 0; i < kFloatEnd; i++) {
		fParas_[i] = 0.5f;
	}

	velocity_ = {};
	vector_ = { 1.0f,0.0f };
	speed_ = 0.0f;

	model_ = std::make_unique<Model>("Cube");
	model_->transform_.scale_ = { 0.4f,0.2f,0.2f };
	SetGlobalVariable();
	model_->Update();

	floatingParameter_ = 0.0f;

	yarn_ = std::make_unique<Yarn>(&model_->transform_.translate_, model_->transform_.translate_);
}

void Player::Initialize()
{
	yarn_.reset(new Yarn(&model_->transform_.translate_, model_->transform_.translate_));
}

void Player::Update()
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG

	Move();

	model_->Update();

	yarn_->Update();
}

void Player::Draw(const Camera* camera)
{
	model_->Draw(*camera);

	yarn_->Draw();
}

void Player::Move()
{
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

		speed_ = std::clamp(speed_ + fParas_[kAcceleration], fParas_[kMinSpeed], fParas_[kMaxSpeed]);

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

		if (speed_ >= fParas_[kMinSpeed]) {
			velocity_ = { vector_.x * speed_,vector_.y * speed_,0.0f };
		}
		else {
			velocity_ = {};
			speed_ = 0.0f;
		}
	}

	model_->transform_.translate_ += velocity_;
}

void Player::InitializeFloating()
{
	floatingParameter_ = 0.0f;
}

void Player::UpdateFloating()
{

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