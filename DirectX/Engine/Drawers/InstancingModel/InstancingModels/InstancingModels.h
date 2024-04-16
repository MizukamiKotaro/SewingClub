#pragma once
#include <list>

#include "Light/Light.h"
#include "Drawers/IDrawer/IDrawer.h"
#include "ModelData/ModelData.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"

#include "InstancingModelData.h"

class Camera;
enum class PipelineType;
class ILight;

class InstancingModels : public IDrawer
{
public:
	static const uint32_t kNumInstance = 100000;

	InstancingModels(const InstancingMeshTexData* modelData);
	~InstancingModels();

	struct Material
	{
		Vector4 color;
		int32_t enableLighting;
		//float padding[3];
	};

	struct ParticleForGPU {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 uvMatrix;
		Vector4 color;
	};

	void Draw(const Camera& camera, std::list<InstancingModelData>& blocks);

	static void PreDraw();

	void SetMesh(const InstancingMeshTexData* modelData);

	void SetLight(const ILight* light);

private:

	void CreateSRV();

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;
	ParticleForGPU* instancingData_;

	Light light_;

private:

	static const PipelineType pipelineType_;
	const InstancingMeshTexData* modelData_;
	const DescriptorHandles* srvHandles_;
};