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

void ParticleManager::LoadModel()
{
	plane_ = ModelDataManager::GetInstance()->LoadObj("Plane");
}

void ParticleManager::Draw(const Camera& camera)
{
	Particle::PreDraw();

	for (const auto& [model, particleList] : instancingModelMap_) {
		if (particleList->GetSize() == 0) { continue; }
		particleList->Draw(camera);
	}
}

ParticleData* const ParticleManager::AddBox(ParticleData&& model, const Texture* texture)
{
	std::pair<const ModelData*, const Texture*> pair = { plane_,texture };
	if (instancingModelMap_.find(pair) == instancingModelMap_.end()) {
		instancingModelMap_[pair] = std::make_unique<ParticleList>(plane_, texture);
	}
	return instancingModelMap_[pair]->AddModel(std::move(model));
}

ParticleData* const ParticleManager::AddBox(ParticleData&& model, const ModelData* modelData, const Texture* texture)
{
	const Texture* tex = texture ? texture : modelData->texture;
	std::pair<const ModelData*, const Texture*> pair = { modelData,tex };
	if (instancingModelMap_.find(pair) == instancingModelMap_.end()) {
		instancingModelMap_[pair] = std::make_unique<ParticleList>(modelData, tex);
	}
	return instancingModelMap_[pair]->AddModel(std::move(model));
}

void ParticleManager::Clear()
{
	for (auto& modelList : instancingModelMap_) {
		modelList.second->Clear();
	}
}

//void ParticleManager::SetLight(const ModelData* modelData, const ILight* light)
//{
//	instancingModelMap_[modelData]->SetLight(light);
//}
