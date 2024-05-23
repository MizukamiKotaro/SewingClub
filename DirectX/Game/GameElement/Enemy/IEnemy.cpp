#include "IEnemy.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "ModelDataManager.h"
#include "TextureManager.h"

ParticleManager* IEnemy::instancingManager_ = nullptr;

void IEnemy::StaticInitialize()
{
	instancingManager_ = ParticleManager::GetInstance();
}

void IEnemy::SetCollider()
{
	Collider::SetCircle({ position_.x,position_.y }, scale_);
	collisionManager_->SetCollider(this);
}

void IEnemy::CreateStageEditor(const std::string& name, const int& no)
{
	std::string tree = "敵" + std::to_string(no);
	int num = no / 10;
	num = num * 10;
	std::string tree1 = "敵" + std::to_string(num) + "～" + std::to_string(num + 9);
	stageEditor_ = std::make_unique<StageEditor>("敵の設置", name, tree1, tree);
	SetGlobalVariable();
}

void IEnemy::DrawDefault(const ParticleMeshTexData* data) const
{
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,0.0f }, position_);
	if (animation_) {
		Matrix4x4 uvMatrix = animation_->GetUVTrans().worldMat_;
		instancingManager_->AddParticle(ParticleData{ matrix,uvMatrix, color_ }, data);
		return;
	}
	instancingManager_->AddParticle(ParticleData{ matrix,Matrix4x4::MakeIdentity4x4(), color_ }, data);
}

const ParticleMeshTexData* IEnemy::CreateData(const std::string& textureName)
{
	return instancingManager_->GetDrawData(
		ParticleMeshTexData{ ModelDataManager::GetInstance()->LoadObj("plane"),
		TextureManager::GetInstance()->LoadTexture(textureName),BlendMode::kBlendModeNormal });
}

