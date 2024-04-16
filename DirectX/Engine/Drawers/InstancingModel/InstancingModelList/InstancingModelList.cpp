#include "InstancingModelList.h"
#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Utils/RandomGenerator/RandomGenerator.h"
#include "DirectXBase/DirectXBase.h"
#include "Camera.h"
#include <numbers>
#include <algorithm>
#include "TextureManager.h"
#include "Light/Light.h"
#include "ModelDataManager.h"

InstancingModelList::InstancingModelList(const InstancingMeshTexData* modelData)
{
	modelsResource_ = std::make_unique<InstancingModels>(modelData);
}

void InstancingModelList::SetLight(const ILight* light) {
	modelsResource_->SetLight(light);
}

void InstancingModelList::Draw(const Camera& camera)
{
	modelsResource_->Draw(camera, modelList_);
}

void InstancingModelList::Clear()
{
	modelList_.clear();
}

InstancingModelData* const InstancingModelList::AddModel(InstancingModelData&& instancingModel)
{
	modelList_.push_back(std::move(instancingModel));
	return &modelList_.back();
}

uint32_t InstancingModelList::GetSize() const
{
	return static_cast<uint32_t>(modelList_.size());
}

void InstancingModelList::SetModel(const InstancingMeshTexData* modelData)
{
	modelsResource_->SetMesh(modelData);
}
