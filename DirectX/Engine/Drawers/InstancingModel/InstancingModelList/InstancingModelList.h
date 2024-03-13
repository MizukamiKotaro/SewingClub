#pragma once
#include <list>
#include <memory>

#include "InstancingModel.h"
#include "InstancingModels/InstancingModels.h"
#include "ILight/ILight.h"

class Camera;

class InstancingModelList {
public:

	InstancingModelList(const ModelData* modelData);

	void Draw(const Camera& camera);

public:

	void Clear();

	InstancingModel* const AddModel(InstancingModel&& instancingModel);

	uint32_t GetSize() const;

	void SetModel(const ModelData* modelData);

	void SetLight(const ILight* light);

private:

	std::unique_ptr<InstancingModels> modelsResource_;
	std::list<InstancingModel> modelList_;
};