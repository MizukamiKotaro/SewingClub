#pragma once
#include <list>

#include "GraphicsPipelineSystem/BlendModeConfig.h"
#include "Light/Light.h"
#include "Drawers/IDrawer/IDrawer.h"
#include "ModelData/ModelData.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"

#include "ParticleData.h"

class Camera;
enum class PipelineType;
class ILight;

class ParticleDrawer : public IDrawer
{
public:
	static const uint32_t kNumInstance = 100000;

	ParticleDrawer(const ParticleMeshTexData& data);
	~ParticleDrawer();

	struct Material
	{
		Vector4 color;
		int32_t enableLighting;
		//float padding[3];
	};

	struct ParticleForGPU {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 uvTransform;
		Vector4 color;
	};

	void Draw(const Camera& camera, std::list<ParticleData>& blocks, BlendMode blendMode = BlendMode::kBlendModeNormal);

	static void PreDraw();

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
	ParticleMeshTexData data_;
	const DescriptorHandles* srvHandles_;
};