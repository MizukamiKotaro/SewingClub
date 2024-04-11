#include "ParticleManager.h"
#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Utils/RandomGenerator/RandomGenerator.h"
#include "DirectXBase/DirectXBase.h"
#include "Camera.h"
#include "Light/Light.h"
#include "ModelDataManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Draw(const Camera& camera)
{
	ParticleDrawer::PreDraw();

	for (const auto& [model, particleList] : particleMap_) {
		if (particleList->GetSize() == 0) { continue; }
		particleList->Draw(camera);
	}
}

void ParticleManager::FirstInitialize()
{
	plane_ = modelDataManager_->LoadObj("plane");
}

const ParticleMeshTexData* ParticleManager::GetDrawData(const ParticleMeshTexData& data)
{
	for (const std::unique_ptr<ParticleMeshTexData>& dataPtr : drawDatas_) {
		if (dataPtr->modelData_ == data.modelData_ && dataPtr->texture_ == data.texture_ && dataPtr->blendMode_ == data.blendMode_) {
			return dataPtr.get();
		}
	}
	drawDatas_.push_back(std::make_unique<ParticleMeshTexData>(data));
	return drawDatas_.back().get();
}

const ParticleMeshTexData* ParticleManager::GetDrawData(const std::string& texturePath, const BlendMode& blendMode)
{
	const Texture* texture = textureManager_->LoadTexture(texturePath);

	for (const std::unique_ptr<ParticleMeshTexData>& dataPtr : drawDatas_) {
		if (dataPtr->modelData_ == plane_ && dataPtr->texture_ == texture && dataPtr->blendMode_ == blendMode) {
			return dataPtr.get();
		}
	}
	drawDatas_.push_back(std::make_unique<ParticleMeshTexData>(ParticleMeshTexData{plane_,texture,blendMode}));
	return drawDatas_.back().get();
}

ParticleData* const ParticleManager::AddParticle(ParticleData&& model, const ParticleMeshTexData* data)
{
	if (particleMap_.find(data) == particleMap_.end()) {
		particleMap_[data] = std::make_unique<ParticleList>(*data);
	}
	return particleMap_[data]->AddModel(std::move(model));
}

void ParticleManager::Clear()
{
	for (auto& particleList : particleMap_) {
		particleList.second->Clear();
	}
}

//void ParticleManager::SetLight(const ModelData* modelData, const ILight* light)
//{
//	instancingModelMap_[modelData]->SetLight(light);
//}
