#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <string>
#include "Drawers/IDrawer/IDrawer.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vector4.h"
#include "Utils/Math/Matrix4x4.h"
#include "GraphicsPipelineSystem/BlendModeConfig.h"

class Camera;
class Texture;
enum class PipelineType;
class GraphicsPipelineManager;

// スプライト
class Sprite : public IDrawer
{
public:

	Sprite(const Vector2& pos = { 640.0f,360.0f }, const Vector2& texLeftTop = {}, const Vector2& texSize = { 1.0f,1.0f },
		const Vector4& color = { 1.0f,1.0f,1.0f,1.0f }, const Vector2& anchorPoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

	Sprite(const std::string& filePath, const Vector2& pos = { 640.0f,360.0f }, const Vector2& texLeftTop = {}, const Vector2& texSize = {1.0f,1.0f},
		const Vector4 & color = { 1.0f,1.0f,1.0f,1.0f }, const Vector2& anchorPoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);
	Sprite(const Texture* texture, const Vector2& pos = { 640.0f,360.0f }, const Vector2& texLeftTop = {}, const Vector2& texSize = {1.0f,1.0f},
		const Vector4& color = { 1.0f,1.0f,1.0f,1.0f }, const Vector2& anchorPoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);
	~Sprite();

	struct VertexData
	{
		Vector4 vertexPos;
		Vector2 texcoord;
	};

	struct Material
	{
		Vector4 color;
		Matrix4x4 uvTransform;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
	};

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static void StaticInitialize();

	void Initialize();

	void Update();

	void Draw(const Camera& camera,  BlendMode blendMode = BlendMode::kBlendModeNormal);

	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal);

public:

	void SetSRVGPUDescriptorHandle_(D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle);

	void LoadTexture(const std::string& filePath);

	void SetTexture(const Texture* texture);

	void SetAnchorPoint(const Vector2& anchorpoint);

	void SetColor(const Vector4& color);

	void SetIsFlipX(bool isFlipX);

	void SetIsFlipY(bool isFlipY);

	void SetTextureTopLeft(const Vector2& texTopLeft);

	void SetTextureSize(const Vector2& texSize);

private:
	static void PreDraw();
	void TransferSize();
	void TransferUV();
	void AdjustTextureSize();
	void CreateVertexRes();
	void CreateMaterialRes();
	void CreateTranformRes();

private:
	ComPtr<ID3D12Resource> vertexResource_;
	VertexData* vertexData_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;
	ComPtr<ID3D12Resource> transformResource_;
	TransformationMatrix* transformData_;

public:
	float rotate_;
	Vector2 pos_;
	Vector2 size_;

private:
	static const PipelineType pipelineType_;

	Matrix4x4 worldMat_;
	Vector2 uvTranslate_;
	Vector2 uvScale_;
	float uvRotate_;

	Vector4 color_;
	Vector2 anchorPoint_;
	Vector2 textureLeftTop_;
	Vector2 textureSize_;

	bool isFlipX_ = false;
	bool isFlipY_ = false;
	bool isInvisible_ = false;

	const Texture* texture_;
	D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle_;
};

