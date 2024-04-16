#include "GravityArea.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"

#ifdef _DEBUG
#include "ModelDataManager.h"

InstancingModelManager* GravityArea::instancingManager_ = nullptr;
const InstancingMeshTexData* GravityArea::modelData_ = nullptr;
#endif // _DEBUG

std::unique_ptr<GlobalVariableUser> GravityArea::globalVariable_;
float GravityArea::scale_ = 1.0f;

GravityArea::GravityArea()
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::GRAVITY_AREA);
	Collider::AddTargetMask(ColliderMask::PLAYER);
	Collider::AddTargetMask(ColliderMask::GRAVITY_AREA_SEARCH);
}

void GravityArea::StaticInitialize()
{
#ifdef _DEBUG
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("WaterCircle");
	modelData_ = instancingManager_->GetDrawData({ modelData,modelData->texture,BlendMode::kBlendModeNormal });
#endif // _DEBUG

	globalVariable_ = std::make_unique<GlobalVariableUser>("Water", "GravityArea");
	globalVariable_->CreateGroup();

	SetGlobalVariable();
}

void GravityArea::StaticUpdate()
{
	ApplyGlobalVariable();
}

void GravityArea::Update(const Vector2& pos, const Vector2& radius, bool isSame, const float& rotate, const Vector2& velcity)
{
	if (isSame) {
		Collider::SetCircle(pos, radius, rotate, velcity);
	}
	else {
		Collider::SetCircle(pos, radius * scale_, rotate, velcity);
	}
	collisionManager_->SetCollider(this);
}


#ifdef _DEBUG
void GravityArea::Draw(const Vector2& pos, const Vector2& radius, bool isSame, const float& rotate)
{
	if (!isSame) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ radius.x * scale_,radius.y * scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate }, Vector3{ pos.x,pos.y,0.0001f });
		instancingManager_->AddBox(modelData_, InstancingModelData{ matrix,Matrix4x4::MakeIdentity4x4(),{1.0f,1.0f,1.0f,0.1f} });
	}
}
#endif // _DEBUG

void GravityArea::SetGlobalVariable()
{
	globalVariable_->AddItem("スケール", scale_);

	ApplyGlobalVariable();
}

void GravityArea::ApplyGlobalVariable()
{
	scale_ = globalVariable_->GetFloatValue("スケール");
}

void GravityArea::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::PLAYER) {

	}
}
