#include "Client.h"
#include "ModelDataManager.h"
#include "GameElement/Planet/PlanetTypeColor.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"

InstancingModelManager* Client::instancingManager_ = nullptr;
const ModelData* Client::modelData_ = nullptr;
std::unique_ptr<GlobalVariableUser> Client::globalVariable_ = nullptr;
float Client::scale_ = 0.5f;

Client::Client(PlanetType type, const Vector3& pos, const Vector3& velocity)
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::RIGID_BODY, ColliderMask::CLIENT);
	Collider::AddTargetMask(ColliderMask::WATER);
	Collider::AddTargetMask(ColliderMask::PLANET);

	type_ = type;
	position_ = pos;
	rotate_ = 0.0f;
	velocity_ = velocity;

	sprite_ = std::make_unique<Sprite>("white.png");
	sprite_->size_ = { 40.0f,40.0f };
	gravityAreaSearch_ = std::make_unique<GravityAreaSearch>();
	isInWater_ = false;
	isInPlanet_ = false;
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
		Vector2 vector = gravityAreaSearch_->GetNearPos() - Vector2{ position_.x,position_.y };
		Vector2 gravityVelocity_ = vector.Normalize() * 0.1f * deltaTime;
		velocity_.x += gravityVelocity_.x;
		velocity_.y += gravityVelocity_.y;
		position_ += velocity_;
	}

	isInWater_ = false;
	SetCollider();
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
	}
	else if (collider.GetMask() == ColliderMask::PLANET) {
		isInPlanet_ = true;
	}
}

void Client::SetCollider()
{
	Collider::SetCircle({ position_.x,position_.y },0.0f);
	collisionManager_->SetCollider(this);
	gravityAreaSearch_->Update(position_);
}

void Client::SetGlobalVariable()
{
}

void Client::ApplyGlobalVariable()
{
}
