#include "Player.h"
#include "Camera.h"
#include "Input.h"
#include "calc.h"
#include <algorithm>
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "GameElement/WaterManager/WaterManager.h"

Player::Player()
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::RIGID_BODY, ColliderMask::PLAYER);
	Collider::AddTargetMask(ColliderMask::WATER);
	Collider::AddTargetMask(ColliderMask::GRAVITY_AREA);

	CreateGlobalVariable("Player");

	input_ = Input::GetInstance();
	waterManager_ = WaterManager::GetInstance();

	for (int i = 0; i < kFloatEnd; i++) {
		fParas_[i] = 0.5f;
	}
	for (int i = 0; i < kBoolEnd; i++) {
		bParas_[i] = false;
	}

	velocity_ = {};
	vector_ = { 1.0f,0.0f };
	speed_ = 0.0f;
	addAcceleration_ = 0.0f;
	timeCount_ = 0.0f;
	gravityVelocity_ = {};
	gravityPos_ = {};
	isGravity_ = false;

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
	isGravity_ = false;
	gravityVelocity_ = {};
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

	if (timeCount_ >= fParas_[kKeepSpeedTime]) {
		addAcceleration_ *= 0.92f;
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
	if (bParas_[BoolParamater::kGravityArea]) {
		if (isGravity_) {
			velocity_.x += gravityVelocity_.x * deltaTime;
			velocity_.y += gravityVelocity_.y * deltaTime;
		}
		else {
			Vector2 vector = gravityPos_ - Vector2{ model_->transform_.translate_.x,model_->transform_.translate_.y };
			gravityVelocity_ = vector.Normalize() * fParas_[kGravityWater];
			velocity_.x += gravityVelocity_.x * deltaTime;
			velocity_.y += gravityVelocity_.y * deltaTime;
		}
	}
	else {
		if (velocity_.y <= 0.0f) {
			velocity_.y -= fParas_[kGravityDown] * deltaTime;
		}
		else {
			velocity_.y -= fParas_[kGravity] * deltaTime;
		}
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

	if (bParas_[BoolParamater::kAddWaterTriger]) {
		if (input_->PressedGamePadButton(Input::GamePadButton::A)) {
			waterManager_->CreateWater({ model_->transform_.translate_.x,model_->transform_.translate_.y },
				{ fParas_[FloatParamater::kWaterSize],fParas_[FloatParamater::kWaterSize] }, false, 0.0f);
		}
	}
}

void Player::Reset()
{
	velocity_ = { 0.0f };
	vector_ = { 1.0f,0.0f };
	speed_ = 0.0f;
	addAcceleration_ = 0.0f;
	timeCount_ = 0.0f;
	gravityVelocity_ = {};
	gravityPos_ = {};
	isGravity_ = false;

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
	else if (collider.GetMask() == ColliderMask::GRAVITY_AREA) {

		ShapeCircle* circle = collider.GetCircle();
		Vector2 pos = { model_->transform_.translate_.x,model_->transform_.translate_.y };
		if (!isGravity_) {
			gravityPos_ = circle->position_;
			isGravity_ = true;
		}
		if ((gravityPos_ - pos).Length() > (circle->position_ - pos).Length()) {
			gravityPos_ = circle->position_;
		}
		
		Vector2 vector = circle->position_ - pos;
		gravityVelocity_ += vector.Normalize() * fParas_[kGravityWater];
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
	for (int i = 0; i < kBoolEnd; i++) {
		globalVariable_->AddItem(bNames[i], bParas_[i]);
	}

	ApplyGlobalVariable();
}

void Player::ApplyGlobalVariable()
{
	model_->transform_.scale_ = globalVariable_->GetVector3Value("スケール");

	for (int i = 0; i < kFloatEnd; i++) {
		fParas_[i] = globalVariable_->GetFloatValue(fNames[i]);
	}
	for (int i = 0; i < kBoolEnd; i++) {
		bParas_[i] = globalVariable_->GetBoolValue(bNames[i]);
	}
}