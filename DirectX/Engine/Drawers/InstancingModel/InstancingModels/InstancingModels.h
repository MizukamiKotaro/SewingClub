#pragma once
#include <list>

#include "GraphicsPipelineSystem/BlendModeConfig.h"
#include "Light/Light.h"
#include "Drawers/IDrawer/IDrawer.h"
#include "ModelData/ModelData.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"

#include "InstancingModel.h"

class Camera;
enum class PipelineType;
class ILight;

class InstancingModels : public IDrawer
{
public:
	static const uint32_t kNumInstance = 100000;

	InstancingModels(const ModelData* modelData);
	~InstancingModels();

	struct Material
	{
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	struct ParticleForGPU {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Vector4 color;
	};

	void Draw(const Camera& camera, std::list<InstancingModel>& blocks, BlendMode blendMode = BlendMode::kBlendModeNormal);

	static void PreDraw();

	void SetMesh(const ModelData* modelData);

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
	const ModelData* modelData_;
	const DescriptorHandles* srvHandles_;
};