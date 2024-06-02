#include "Vignette.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

Vignette::Vignette()
{
	piplineType_ = PipelineType::VIGNETTE;
	vignetteData_ = nullptr;
	CreatePostEffect();
}

Vignette::~Vignette()
{
	vignetteResource_->Release();
}

void Vignette::Draw(BlendMode blendMode)
{
	materialData_->color = color_;

	PreDraw();
	psoManager_->SetBlendMode(piplineType_, blendMode);
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);
	commandList_->SetGraphicsRootConstantBufferView(2, vignetteResource_->GetGPUVirtualAddress());
	commandList_->DrawInstanced(3, 1, 0, 0);
}

void Vignette::CreateVignetteRes()
{
	vignetteResource_ = DirectXBase::CreateBufferResource(sizeof(VignetteData));
	vignetteResource_->Map(0, nullptr, reinterpret_cast<void**>(&vignetteData_));
	vignetteData_->scale = 16.0f;
	vignetteData_->powNum = 0.8f;
}

void Vignette::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateVignetteRes();
}