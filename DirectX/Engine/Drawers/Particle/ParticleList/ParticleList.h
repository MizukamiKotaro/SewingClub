#pragma once
#include <list>
#include <memory>

#include "ParticleData.h"
#include "ParticleDrawer/ParticleDrawer.h"
#include "ILight/ILight.h"

class Camera;

class ParticleList {
public:

	ParticleList(const ParticleMeshTexData& data);

	void Draw(const Camera& camera);

	void Clear();

	ParticleData* const AddModel(ParticleData&& data);

	uint32_t GetSize() const;

	void SetLight(const ILight* light);

private:

	std::unique_ptr<ParticleDrawer> drawer_;
	std::list<ParticleData> dataList_;
};