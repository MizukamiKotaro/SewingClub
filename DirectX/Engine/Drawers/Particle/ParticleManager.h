#pragma once
#include <list>
#include <unordered_map>
#include <map>
#include <memory>
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"

#include "Light/Light.h"
#include "ModelData/ModelData.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "ILight/ILight.h"

#include "ParticleData.h"
#include "ParticleList/ParticleList.h"

class Camera;

class ParticleManager
{
public:

	static ParticleManager* GetInstance();

	void LoadModel();

	void Draw(const Camera& camera);

	ParticleData* const AddBox(ParticleData&& model, const Texture* texture);
	ParticleData* const AddBox(ParticleData&& model, const  ModelData* modelData, const Texture* texture = nullptr);

	void Clear();

	//void SetLight(const ModelData* modelData, const ILight* light);

private:
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	const ParticleManager& operator=(const ParticleManager&) = delete;

private:
	const ModelData* plane_;

	std::map<std::pair<const ModelData*,const Texture*>, std::unique_ptr<ParticleList>> instancingModelMap_;
};