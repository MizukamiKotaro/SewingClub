#include "Baby.h"
#include "ModelDataManager.h"
#include "GameElement/Planet/PlanetTypeColor.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"

Baby::Baby(const Vector3& pos, const Vector3& velocity)
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::UNKNOWN, ColliderMask::BABY);
	Collider::AddTargetMask(ColliderMask::WATER);
	Collider::AddTargetMask(ColliderMask::PLANET);
	Collider::AddTargetMask(ColliderMask::GRAVITY_AREA);
	Collider::AddTargetMask(ColliderMask::PLAYER);

	position_ = pos;
	rotate_ = 0.0f;
	velocity_ = velocity;
	
}

void Baby::Initialize()
{

}

void Baby::Update(float deltaTime)
{
	deltaTime = deltaTime;
}

void Baby::Draw() const
{
	/*Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
	instancingManager_->AddBox(modelData_, InstancingModel{ matrix,PlanetTypeColor::GetColor(type_) });*/
}

void Baby::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::WATER) {

	}
}

void Baby::SetCollider()
{
	Collider::SetCircle({ position_.x,position_.y }, scale_);
	collisionManager_->SetCollider(this);
}

void Baby::SetGlobalVariable()
{
	/*globalVariable_->AddItem("重力加速度", gravitySpeed_);
	globalVariable_->AddItem("スケール", scale_);
	globalVariable_->AddItem("水や惑星内の重力加速度", planetGravitySpeed_);
	ApplyGlobalVariable();*/
}

void Baby::ApplyGlobalVariable()
{
	/*gravitySpeed_ = globalVariable_->GetFloatValue("重力加速度");
	scale_ = globalVariable_->GetFloatValue("スケール");
	planetGravitySpeed_ = globalVariable_->GetFloatValue("水や惑星内の重力加速度");*/
}
