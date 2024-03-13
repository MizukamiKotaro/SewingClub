#include "InstancingModelManager.h"
#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Utils/RandomGenerator/RandomGenerator.h"
#include "DirectXBase/DirectXBase.h"
#include "Camera.h"
#include "Light/Light.h"
#include "ModelDataManager.h"

InstancingModelManager* InstancingModelManager::GetInstance()
{
	static InstancingModelManager instance;
	return &instance;
}

void InstancingModelManager::Draw(const Camera& camera)
{
	InstancingModels::PreDraw();

	for (const auto& [model, particleList] : instancingModelMap_) {
		if (particleList->GetSize() == 0) { continue; }
		particleList->Draw(camera);
	}
}

InstancingModel* const InstancingModelManager::AddBox(const ModelData* modelData, InstancingModel&& model)
{
	if (instancingModelMap_.find(modelData) == instancingModelMap_.end()) {
		instancingModelMap_[modelData] = std::make_unique<InstancingModelList>(modelData);
		instancingModelMap_[modelData]->SetModel(modelData);
	}
	return instancingModelMap_[modelData]->AddModel(std::move(model));
}

void InstancingModelManager::Clear()
{
	for (auto& modelList : instancingModelMap_) {
		modelList.second->Clear();
	}
}

void InstancingModelManager::SetLight(const ModelData* modelData, const ILight* light)
{
	instancingModelMap_[modelData]->SetLight(light);
}
