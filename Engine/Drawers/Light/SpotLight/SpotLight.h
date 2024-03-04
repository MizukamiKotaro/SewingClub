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

class SpotLight : public ILight, public IDrawer
{
public:
	struct SpotLightData
	{
		Vector4 color; // ライトの色
		Vector3 position; // 位置
		float intensity; // 輝度
		Vector3 direction; // スポットライトの方向
		float distance; // ライトの届く距離
		float decay; // 減衰率
		float cosAngle; // スポットライトの余弦
		float cosFalloffStart;
		float padding;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverse;
	};

	SpotLight();
	~SpotLight() override;

	static void StaticInitialize();

	void Update() override;

	void Draw(const Camera& camera, BlendMode blendMode = BlendMode::kBlendModeNormal);

private:
	void CreateTransformationResource();

public:
	SpotLightData* light_ = nullptr;
	bool isDraw_;

private:
	static const PipelineType piplineType_;

	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource_;
	TransformationMatrix* transformationData_;

	static const ModelData* modelData_;
	static const Matrix4x4 scaleMat_;
	static const Matrix4x4 scaleInverseMat_;
};
