#include "Dissolve.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

Dissolve::Dissolve()
{
	piplineType_ = PipelineType::DISSOLVE;
	dissolveData_ = nullptr;
	CreatePostEffect();
}

Dissolve::~Dissolve()
{
	dissolveResource_->Release();
}

void Dissolve::Draw(BlendMode blendMode)
{
	materialData_->color = color_;

	PreDraw();
	psoManager_->SetBlendMode(piplineType_, blendMode);
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);
	commandList_->SetGraphicsRootConstantBufferView(2, dissolveResource_->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootDescriptorTable(3, maskTextureHandle_);
	commandList_->DrawInstanced(3, 1, 0, 0);
}

void Dissolve::SetGPUDescriptorHandle(const D3D12_GPU_DESCRIPTOR_HANDLE& mask)
{
	maskTextureHandle_ = mask;
}

void Dissolve::CreateDissolveRes()
{
	dissolveResource_ = DirectXBase::CreateBufferResource(sizeof(DissolveData));
	dissolveResource_->Map(0, nullptr, reinterpret_cast<void**>(&dissolveData_));
	dissolveData_->edgeColor = { 1.0f,0.4f,0.3f };
	dissolveData_->baseLuminance = 1.0f;
	dissolveData_->difference = 0.03f;
}

void Dissolve::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateDissolveRes();
}