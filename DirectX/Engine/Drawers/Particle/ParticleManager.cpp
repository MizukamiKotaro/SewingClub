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

ParticleData* const ParticleManager::AddBox(ParticleData&& model, const ParticleMeshTexData* data)
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
