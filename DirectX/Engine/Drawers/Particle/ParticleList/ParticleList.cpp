#include "ParticleList.h"
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

ParticleList::ParticleList(const ModelData* modelData, const Texture* texture)
{
	modelsResource_ = std::make_unique<Particle>(modelData, texture);
}

void ParticleList::SetLight(const ILight* light) {
	modelsResource_->SetLight(light);
}

void ParticleList::Draw(const Camera& camera)
{
	modelsResource_->Draw(camera, modelList_);
}

void ParticleList::Clear()
{
	modelList_.clear();
}

ParticleData* const ParticleList::AddModel(ParticleData&& instancingModel)
{
	modelList_.push_back(std::move(instancingModel));
	return &modelList_.back();
}

uint32_t ParticleList::GetSize() const
{
	return static_cast<uint32_t>(modelList_.size());
}
