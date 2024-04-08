#pragma once
#include <list>
#include <memory>

#include "ParticleData.h"
#include "Particle/Particle.h"
#include "ILight/ILight.h"

class Camera;

class ParticleList {
public:

	ParticleList(const ModelData* modelData, const Texture* texture);

	void Draw(const Camera& camera);

	void Clear();

	ParticleData* const AddModel(ParticleData&& instancingModel);

	uint32_t GetSize() const;

	void SetLight(const ILight* light);

private:

	std::unique_ptr<Particle> modelsResource_;
	std::list<ParticleData> modelList_;
};