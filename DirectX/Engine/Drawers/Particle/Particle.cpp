#include "Particle.h"
#include "ParticleManager.h"
#include "ModelDataManager.h"
#include "TextureManager.h"
#include "Camera.h"

const ModelData* Particle::plane_ = nullptr;
ParticleManager* Particle::particleManager_ = nullptr;



Particle::Particle(const std::string& particleName, const std::string& textureName, bool isStageEditor, BlendMode blendMode)
{
	drawData_ = particleManager_->GetDrawData(ParticleMeshTexData{ plane_,textureManager_->LoadTexture(textureName),blendMode });
	
	if (isStageEditor) {
		stageEditor_ = std::make_unique<StageEditor>("パーティクルの設置");
	}
	else {
		globalVariable_ = std::make_unique<GlobalVariableUser>("Particle", particleName);
	}
	SetGlobalVariable();


}

Particle::Particle(const std::string& particleName, const std::string& modelName, const std::string& textureName, bool isStageEditor, BlendMode blendMode)
{
	if (textureName == "_") {
		const ModelData* modelData = modelDataManager_->LoadObj(modelName);
		drawData_ = particleManager_->GetDrawData(ParticleMeshTexData{ modelData, modelData->texture,blendMode });
	}
	else {
		drawData_ = particleManager_->GetDrawData(ParticleMeshTexData{ modelDataManager_->LoadObj(modelName),textureManager_->LoadTexture(textureName),blendMode});
	}
	if (isStageEditor) {
		stageEditor_ = std::make_unique<StageEditor>("パーティクルの設置", particleName);
	}
	else {
		globalVariable_ = std::make_unique<GlobalVariableUser>("Particle", particleName);
	}
	SetGlobalVariable();
}

void Particle::StaticInitialize()
{
	particleManager_ = ParticleManager::GetInstance();
	plane_ = ModelDataManager::GetInstance()->LoadObj("Plane");
}

void Particle::Initialize()
{
}

void Particle::Update(float deltaTime, Camera* camera)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG


	deltaTime = deltaTime;
	camera->transform_ = camera->transform_;
}

void Particle::Draw(Camera* camera)
{
	camera->transform_ = camera->transform_;
}

void Particle::SetGlobalVariable()
{
	if (globalVariable_) {

	}
	if (stageEditor_) {

	}

	ApplyGlobalVariable();
}

void Particle::ApplyGlobalVariable()
{
	if (globalVariable_) {

	}
	if (stageEditor_) {

	}
}
