#pragma once

#include <wrl.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")
#include <string>
#include <d3d12.h>
#include <unordered_map>
#include <memory>
#include <vector>

enum class BlendMode;

class GraphicsPipeline
{
public:
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize();

	void PreDraw();

	void SetBlendMode(BlendMode blendMode);

	virtual ~GraphicsPipeline() = default;
protected:

	void InitializeDXC();

	virtual void InitializePSO() = 0;

	void CreatePSO(BlendMode blendMode);

	D3D12_BLEND_DESC CreateBlendDesc(BlendMode blendMode);

	//コンパイル用関数
	IDxcBlob* CompileShader(
		//compilerするShaderファイルへパス
		const std::wstring& filePath,
		//compilerに使用するprofile
		const wchar_t* profile,
		//初期化で生成したもの3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);
protected:

	ID3D12Device* device_ = nullptr;
	ID3D12GraphicsCommandList* commandList_ = nullptr;

	ComPtr<IDxcUtils> dxcUtils_;
	ComPtr<IDxcCompiler3> dxcCompiler_;
	ComPtr<IDxcIncludeHandler> includeHandler_;

	ComPtr<ID3DBlob> signatureBlob_;
	ComPtr<ID3DBlob> errorBlob_;
	ComPtr<ID3D12RootSignature> rootSignature_;

	ComPtr<IDxcBlob> vertexShaderBlob_;
	ComPtr<IDxcBlob> pixelShaderBlob_;

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
	D3D12_RASTERIZER_DESC rasterizerDesc;
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc;
	
	std::unordered_map<BlendMode,ComPtr<ID3D12PipelineState>> graphicsPipelineStates_;

	BlendMode blendMode_;
};

