#pragma once
#include <list>
#include <unordered_map>
#include <memory>
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"

#include "Light/Light.h"
#include "ModelData/ModelData.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "ILight/ILight.h"

#include "InstancingModel.h"
#include "InstancingModelList/InstancingModelList.h"

class Camera;

class InstancingModelManager
{
public:

	static InstancingModelManager* GetInstance();

	void Draw(const Camera& camera);

	InstancingModel* const AddBox(const ModelData* modelData, InstancingModel&& model);

	void Clear();

	void SetLight(const ModelData* modelData, const ILight* light);

private:
	InstancingModelManager() = default;
	~InstancingModelManager() = default;
	InstancingModelManager(const InstancingModelManager&) = delete;
	const InstancingModelManager& operator=(const InstancingModelManager&) = delete;

private:
	std::unordered_map<const ModelData*, std::unique_ptr<InstancingModelList>> instancingModelMap_;
};