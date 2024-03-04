#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "ILight/ILight.h"
#include "Drawers/IDrawer/IDrawer.h"
#include "GraphicsPipelineSystem/BlendModeConfig.h"

class Camera;
class ModelData;
enum class PipelineType;

class PointLight : public ILight, public IDrawer
{
public:
	struct PointLightData
	{
		Vector4 color; // ライトの色
		Vector3 position; // 位置
		float intensity; // 輝度
		float radius; // ライトの届く距離
		float decay; // 減衰率
		float padding[2];
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverse;
	};

	PointLight();
	~PointLight() override;

	static void StaticInitialize();

	void Update() override;

	void Draw(const Camera& camera, BlendMode blendMode = BlendMode::kBlendModeNormal);

private:
	void CreateTransformationResource();

public:
	PointLightData* light_ = nullptr;
	bool isDraw_;

private:
	static const PipelineType pipelineType_;

	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource_;
	TransformationMatrix* transformationData_;

	static const ModelData* modelData_;
	static const Matrix4x4 scaleMat_;
	static const Matrix4x4 scaleInverseMat_;
};
