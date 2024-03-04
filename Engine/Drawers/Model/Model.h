#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <string>
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vector4.h"
#include "Utils/Math/Matrix4x4.h"
#include "Utils/Transform/Transform.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "Light/Light.h"
#include "ModelData/ModelData.h"
#include "ModelDataManager.h"
#include "Texture.h"
#include "GraphicsPipelineSystem/BlendModeConfig.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"

#include "Drawers/IDrawer/IDrawer.h"

class Camera;

class Model : public IDrawer
{
public:

	// Model(const std::string& fileName);
	// 例)
	// Cubeフォルダ
	//   Cube.obj
	//   Cube.mtl
	// の状態

	/// <summary>
	/// モデルの生成
	/// </summary>
	/// <param name="fileName">.objが入っているフォルダの名前( "Cube" 等)</param>
	Model(const std::string& fileName);

	/// <summary>
	/// モデルの生成
	/// </summary>
	/// <param name="meshHundle">モデルデータのハンドル( ModelDataManager::GetInstance()->LoadObj("Cube") 等)</param>
	Model(const ModelData* modelData);
	~Model();

	struct Material
	{
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess; // 光沢度
		Vector3 supeqularColor; // 鏡面反射の色
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverse;
	};

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static void StaticInitialize();

	void Initialize();

	void Update();

	void Draw(const Camera& camera, BlendMode blendMode = BlendMode::kBlendModeNormal);


private:
	static void PreDraw();

public:

	void SetTexture(const Texture* texture);

	void SetSRVGPUDescriptorHandle_(const D3D12_GPU_DESCRIPTOR_HANDLE& srvGPUDescriptorHandle) { srvGPUDescriptorHandle_ = srvGPUDescriptorHandle; }

	void SetModelData(const ModelData* modelData);

	void SetLight(const ILight* light);

	void UnUsedLight() { materialData_->enableLighting = 0; }

	void SetColor(const Vector4& color) { color_ = color; }

private:
	ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	ComPtr<ID3D12Resource> transformationResource_;
	TransformationMatrix* transformationData_;

private:

	void CreateResources();

	void CreateMaterialResource();

	void CreateTransformationResource();

	void InitVariables();

public:
	Transform transform_;
	Vector4 color_;

private:
	static const PipelineType pipelineType_ = PipelineType::MODEL;

	Light light_;

	Matrix4x4 uvMatrix_;

	Vector3 uvScale_;
	Vector3 uvRotate_;
	Vector3 uvPos_;

	const ModelData* modelData_;

	// texture変えたい時用
	const Texture* texture_;

	// ポストエフェクト用
	D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle_;
};