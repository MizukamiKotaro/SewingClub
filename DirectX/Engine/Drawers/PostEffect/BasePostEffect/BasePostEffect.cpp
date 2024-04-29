#include "BasePostEffect.h"

#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "WindowsInfo/WindowsInfo.h"
#include "Externals/DirectXTex/d3dx12.h"
#include <algorithm>
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

const float BasePostEffect::clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

DescriptorHeap* BasePostEffect::srvHeap_ = nullptr;
DescriptorHeap* BasePostEffect::rtvHeap_ = nullptr;
DescriptorHeap* BasePostEffect::dsvHeap_ = nullptr;
ID3D12Device* BasePostEffect::device_ = nullptr;

BasePostEffect::~BasePostEffect()
{
	materialResource_->Release();
	srvHeap_->DeleteDescriptor(srvHandles_);
	rtvHeap_->DeleteDescriptor(rtvHandles_);
	dsvHeap_->DeleteDescriptor(dsvHandles_);
}

void BasePostEffect::StaticInitialize()
{
	DescriptorHeapManager* descriptorHeapManager = DescriptorHeapManager::GetInstance();
	srvHeap_ = descriptorHeapManager->GetSRVDescriptorHeap();
	rtvHeap_ = descriptorHeapManager->GetRTVDescriptorHeap();
	dsvHeap_ = descriptorHeapManager->GetDSVDescriptorHeap();
	device_ = DirectXBase::GetInstance()->GetDevice();
}

void BasePostEffect::Initialize()
{
	
}

void BasePostEffect::Update()
{
	
}

void BasePostEffect::Draw(BlendMode blendMode)
{

	PreDraw();

	psoManager_->SetBlendMode(piplineType_, blendMode);

	//Spriteの描画。変更に必要なものだけ変更する
	//commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	//マテリアルCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	//commandList_->SetGraphicsRootConstantBufferView(1, transformResource_->GetGPUVirtualAddress());

	commandList_->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);

	//描画!!!!（DrawCall/ドローコール）
	commandList_->DrawInstanced(3, 1, 0, 0);

}

void BasePostEffect::PreDrawScene()
{
	// バリアの変更
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(texResource_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList_->ResourceBarrier(1, &barrier);

	// レンダーターゲットのセット
	commandList_->OMSetRenderTargets(1, &rtvHandles_->cpuHandle, false, &dsvHandles_->cpuHandle);

	// ビューポートの設定
	CD3DX12_VIEWPORT viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, size_.x, size_.y);
	commandList_->RSSetViewports(1, &viewport);

	// シザリング矩形の設定
	CD3DX12_RECT rect = CD3DX12_RECT(0, 0, (UINT)size_.x, (UINT)size_.y);
	commandList_->RSSetScissorRects(1, &rect);

	// 全画面クリア
	commandList_->ClearRenderTargetView(rtvHandles_->cpuHandle, clearColor, 0, nullptr);

	// 深度バッファクリア
	commandList_->ClearDepthStencilView(dsvHandles_->cpuHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvHeap_->GetHeap() };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps);

	psoManager_->PreDraw();
}

void BasePostEffect::PostDrawScene()
{
	// バリアの変更
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(texResource_.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	commandList_->ResourceBarrier(1, &barrier);
}

void BasePostEffect::CreateMaterialRes()
{
	//マテリアル用のリソースを作る。今回はcolor1つ分を用意する
	materialResource_ = DirectXBase::CreateBufferResource(sizeof(Material));
	//マテリアルデータを書き込む
	//書き込むためのアドレスを取得l
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//今回は赤を書き込んでいる
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	color_ = { 1.0f,1.0f,1.0f,1.0f };
}


void BasePostEffect::CreateTexRes()
{

	CD3DX12_RESOURCE_DESC texDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		(UINT)size_.x,
		(UINT)size_.y,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	CD3DX12_HEAP_PROPERTIES properties = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	CD3DX12_CLEAR_VALUE value = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

	HRESULT hr = device_->CreateCommittedResource(
		&properties,
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&value,
		IID_PPV_ARGS(&texResource_)
	);
	assert(SUCCEEDED(hr));

	// イメージデータの転送

	// 画素数
	const UINT pixelCount = (UINT)size_.x * (UINT)size_.y;
	// 画像1行分のデータ
	const UINT rowPitch = sizeof(UINT) * (UINT)size_.x;
	// 画像全体のデータサイズ
	const UINT depthPitch = rowPitch * (UINT)size_.y;

	// 画像イメージ
	UINT* img = new UINT[pixelCount];
	for (UINT i = 0; i < pixelCount; i++) { img[i] = 0xFF0000FF; }

	// データの転送
	hr = texResource_->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
	assert(SUCCEEDED(hr));
	delete[] img;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	srvHandles_ = srvHeap_->GetNewDescriptorHandles();

	device_->CreateShaderResourceView(texResource_.Get(), &srvDesc, srvHandles_->cpuHandle);
}

void BasePostEffect::CreateRTV()
{
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; //2dテクスチャとして書き込む

	rtvHandles_ = rtvHeap_->GetNewDescriptorHandles();

	device_->CreateRenderTargetView(texResource_.Get(), &rtvDesc, rtvHandles_->cpuHandle);
}

void BasePostEffect::CreateDSV()
{
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		(UINT)size_.x,
		(UINT)size_.y,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	CD3DX12_HEAP_PROPERTIES properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE value = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D24_UNORM_S8_UINT, 1.0f, 0);

	HRESULT hr = device_->CreateCommittedResource(
		&properties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&value,
		IID_PPV_ARGS(&dsvResource_)
	);
	assert(SUCCEEDED(hr));

	dsvHandles_ = dsvHeap_->GetNewDescriptorHandles();

	//DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2dTexture
	// DSVHeapの先頭にDSVを作る
	device_->CreateDepthStencilView(dsvResource_.Get(), &dsvDesc, dsvHandles_->cpuHandle);
}

void BasePostEffect::PreDraw() const
{
	psoManager_->PreDraw(piplineType_);
}

void BasePostEffect::CreateResources()
{
	CreateMaterialRes();

	CreateTexRes();

	CreateRTV();

	CreateDSV();
}

const D3D12_GPU_DESCRIPTOR_HANDLE BasePostEffect::GetSRVGPUDescriptorHandle() const
{
	return srvHandles_->gpuHandle;
}

void BasePostEffect::CreatePostEffect()
{
	size_ = WindowsInfo::GetInstance()->GetWindowSize();

	CreateResources();

	Update();
}