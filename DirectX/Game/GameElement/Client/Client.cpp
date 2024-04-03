#include "Client.h"
#include "ModelDataManager.h"
#include "GameElement/Planet/PlanetTypeColor.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"

InstancingModelManager* Client::instancingManager_ = nullptr;
const ModelData* Client::modelData_ = nullptr;
std::unique_ptr<GlobalVariableUser> Client::globalVariable_ = nullptr;
float Client::scale_ = 0.5f;
float Client::gravitySpeed_ = 0.5f;
float Client::planetGravitySpeed_ = 0.1f;

Client::Client(PlanetType type, const Vector3& pos, const Vector3& velocity)
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::UNKNOWN, ColliderMask::CLIENT);
	Collider::AddTargetMask(ColliderMask::WATER);
	Collider::AddTargetMask(ColliderMask::PLANET);
	Collider::AddTargetMask(ColliderMask::GRAVITY_AREA);
	Collider::AddTargetMask(ColliderMask::PLAYER);

	type_ = type;
	position_ = pos;
	rotate_ = 0.0f;
	velocity_ = velocity;

	sprite_ = std::make_unique<Sprite>("white.png");
	sprite_->size_ = { 40.0f,40.0f };
	gravityAreaSearch_ = std::make_unique<GravityAreaSearch>();
	isInWater_ = false;
	isInPlanet_ = false;
	isInGravity_ = false;
	gravityPos_ = {};
	gravityVelocity_ = {};
	timeCount_ = 0.0f;
}

void Client::StaticInitialize()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	modelData_ = ModelDataManager::GetInstance()->LoadObj("WaterCircle");
	globalVariable_ = std::make_unique<GlobalVariableUser>("Charactor", "Client");
	globalVariable_->CreateGroup();

	SetGlobalVariable();
}

void Client::Initialize()
{

}

void Client::Update(float deltaTime)
{
	if (!isInWater_) {
		if (isInGravity_) {
			velocity_.x += gravityVelocity_.x * deltaTime;
			velocity_.y += gravityVelocity_.y * deltaTime;
			position_ += velocity_;
		}
		else {
			Vector2 vector = gravityAreaSearch_->GetNearPos() - Vector2{ position_.x,position_.y };
			Vector2 gravityVelocity = vector.Normalize() * gravitySpeed_ * deltaTime;
			velocity_.x += gravityVelocity.x;
			velocity_.y += gravityVelocity.y;
			position_ += velocity_;
		}
	}


	isInGravity_ = false;
	isInWater_ = false;
	gravityVelocity_ = {};

	timeCount_ += deltaTime;
	if (timeCount_ >= 0.4f) {
		Collider::type_ = ColliderType::COLLIDER;
		SetCollider();
	}
}

void Client::Draw() const
{
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
	instancingManager_->AddBox(modelData_, InstancingModel{ matrix,PlanetTypeColor::GetColor(type_) });
}

void Client::Draw(const Vector2& pos) const
{
	sprite_->pos_ = pos;
	sprite_->SetColor(PlanetTypeColor::GetColor(type_));
	sprite_->Update();
	sprite_->Draw();
}

void Client::StaticUpdate()
{
	ApplyGlobalVariable();
}

void Client::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::WATER) {
		isInWater_ = true;
		velocity_ = {};
	}
	else if (collider.GetMask() == ColliderMask::PLANET) {
		isInPlanet_ = true;
	}
	else if (collider.GetMask() == ColliderMask::GRAVITY_AREA) {
		ShapeCircle* circle = collider.GetCircle();
		Vector2 pos = { position_.x,position_.y };
		if (!isInGravity_) {
			gravityPos_ = circle->position_;
			isInGravity_ = true;
		}
		if ((gravityPos_ - pos).Length() > (circle->position_ - pos).Length()) {
			gravityPos_ = circle->position_;
		}

		Vector2 vector = circle->position_ - pos;
		gravityVelocity_ += vector.Normalize() * planetGravitySpeed_;
	}
	else if (collider.GetMask() == ColliderMask::PLAYER) {

	}
}

void Client::SetCollider()
{
	Collider::SetCircle({ position_.x,position_.y }, scale_);
	collisionManager_->SetCollider(this);
	gravityAreaSearch_->Update(position_);
}

void Client::SetGlobalVariable()
{
	globalVariable_->AddItem("重力加速度", gravitySpeed_);
	globalVariable_->AddItem("スケール", scale_);
	globalVariable_->AddItem("水や惑星内の重力加速度", planetGravitySpeed_);
	ApplyGlobalVariable();
}

void Client::ApplyGlobalVariable()
{
	gravitySpeed_ = globalVariable_->GetFloatValue("重力加速度");
	scale_ = globalVariable_->GetFloatValue("スケール");
	planetGravitySpeed_ = globalVariable_->GetFloatValue("水や惑星内の重力加速度");
}
