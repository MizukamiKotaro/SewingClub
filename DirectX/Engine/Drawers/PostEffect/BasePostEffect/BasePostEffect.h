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

class DescriptorHandles;
enum class PipelineType;
class GraphicsPipelineManager;
class DescriptorHeap;

class BasePostEffect : public IDrawer
{
public:
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	virtual ~BasePostEffect();
	static void StaticInitialize();
	virtual void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画処理,ポストエフェクトさせたものの描画
	/// </summary>
	virtual void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal);
	/// <summary>
	/// 描画前の処理,ポストエフェクトさせるものの描画処理の前に書く,更新処理の部分に書くのが吉
	/// </summary>
	void PreDrawScene();
	/// <summary>
	/// 描画後の処理,ポストエフェクトさせるものの描画処理の後に書く,更新処理の部分に書くのが吉
	/// </summary>
	void PostDrawScene();
protected:
	void PreDraw() const;
	virtual void CreateResources();
public:
	/// <summary>
	/// SRVGPUDescriptorHandleの取得,モデルなどにtextureとして渡す用
	/// </summary>
	/// <returns>SRVGPUDescriptorHandle</returns>
	const D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle() const;
protected:
	struct VertexData
	{
		Vector4 vertexPos;
		Vector2 texcoord;
	};
	struct Material
	{
		Vector4 color;
	};

	void CreatePostEffect();
private:
	void CreateMaterialRes();
	void CreateTexRes();
	void CreateRTV();
	void CreateDSV();
protected:
	ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;
	ComPtr<ID3D12Resource> texResource_;
	const DescriptorHandles* srvHandles_;
	ComPtr<ID3D12Resource> rtvResource_;
	const DescriptorHandles* rtvHandles_;
	ComPtr<ID3D12Resource> dsvResource_;
	const DescriptorHandles* dsvHandles_;
public:
	Vector4 color_;
protected:
	static DescriptorHeap* srvHeap_;
	static DescriptorHeap* rtvHeap_;
	static DescriptorHeap* dsvHeap_;
	static ID3D12Device* device_;
	PipelineType piplineType_;
	static const float clearColor[4];
	Vector2 size_;
};