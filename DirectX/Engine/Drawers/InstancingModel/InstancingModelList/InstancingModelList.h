#pragma once
#include <list>
#include <memory>

#include "InstancingModelData.h"
#include "InstancingModels/InstancingModels.h"
#include "ILight/ILight.h"

class Camera;

class InstancingModelList {
public:

	InstancingModelList(const InstancingMeshTexData* modelData);

	void Draw(const Camera& camera);

public:

	void Clear();

	InstancingModelData* const AddModel(InstancingModelData&& instancingModel);

	uint32_t GetSize() const;

	void SetModel(const InstancingMeshTexData* modelData);

	void SetLight(const ILight* light);

private:

	std::unique_ptr<InstancingModels> modelsResource_;
	std::list<InstancingModelData> modelList_;
};