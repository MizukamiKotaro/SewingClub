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
#include "Drawers/IDrawer/IDrawer.h"

class Camera;

class ParticleManager : public IDrawer
{
public:

	static ParticleManager* GetInstance();

	void Draw(const Camera& camera);

	void FirstInitialize();

	const ParticleMeshTexData* GetDrawData(const ParticleMeshTexData& data);
	// 板ポリの場合
	const ParticleMeshTexData* GetDrawData(const std::string& texturePath, const BlendMode& blendMode = BlendMode::kBlendModeNormal);

	ParticleData* const AddParticle(ParticleData&& model, const ParticleMeshTexData* data);

	void Clear();

	//void SetLight(const ModelData* modelData, const ILight* light);

private:
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	const ParticleManager& operator=(const ParticleManager&) = delete;

private:
	std::unordered_map<const ParticleMeshTexData*, std::unique_ptr<ParticleList>> particleMap_;
	std::list<std::unique_ptr<ParticleMeshTexData>> drawDatas_;
	const ModelData* plane_;
};