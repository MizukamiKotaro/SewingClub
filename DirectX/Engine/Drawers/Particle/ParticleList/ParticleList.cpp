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

ParticleList::ParticleList(const ParticleMeshTexData& data)
{
	drawer_ = std::make_unique<ParticleDrawer>(data);
}

void ParticleList::SetLight(const ILight* light) {
	drawer_->SetLight(light);
}

void ParticleList::Draw(const Camera& camera)
{
	drawer_->Draw(camera, dataList_);
}

void ParticleList::Clear()
{
	dataList_.clear();
}

ParticleData* const ParticleList::AddModel(ParticleData&& data)
{
	dataList_.push_back(std::move(data));
	return &dataList_.back();
}

uint32_t ParticleList::GetSize() const
{
	return static_cast<uint32_t>(dataList_.size());
}
