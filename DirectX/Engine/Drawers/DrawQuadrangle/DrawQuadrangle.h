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
class DrawQuadrangle : public IDrawer
{
public:

	DrawQuadrangle();

	~DrawQuadrangle();

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

	void Draw(const Camera& camera, const Vector4& color, const Texture& texture, BlendMode blendMode = BlendMode::kBlendModeNormal);

	void SetVertex(const Vector3& leftTop, const Vector3& rightTop, const Vector3& leftBottom, const Vector3& rightBottom);
private:
	void TransferUV();
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

};

