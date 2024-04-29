#include "GaussianBlur.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

GaussianBlur::GaussianBlur()
{
	piplineType_ = PipelineType::GAUSSIAN_BLUR;

	gaussianBlurData_ = nullptr;

	CreatePostEffect();
}

GaussianBlur::~GaussianBlur()
{
	gaussianBlurResource_->Release();
}

void GaussianBlur::Draw(BlendMode blendMode)
{
	if (gaussianBlurData_->pickRange <= 0.0f) {
		gaussianBlurData_->pickRange = 0.001f;
	}
	if (gaussianBlurData_->stepWidth <= 0.0f) {
		gaussianBlurData_->stepWidth = 0.0001f;
	}

	PreDraw();

	psoManager_->SetBlendMode(piplineType_, blendMode);

	ID3D12GraphicsCommandList* commandList = DirectXBase::GetInstance()->GetCommandList();

	//Spriteの描画。変更に必要なものだけ変更する
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);

	commandList->SetGraphicsRootConstantBufferView(2, gaussianBlurResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(3, 1, 0, 0);

}

void GaussianBlur::CreateGaussianBlurRes()
{
	gaussianBlurResource_ = DirectXBase::CreateBufferResource(sizeof(GaussianBlurData));

	gaussianBlurResource_->Map(0, nullptr, reinterpret_cast<void**>(&gaussianBlurData_));

	gaussianBlurData_->pickRange = 0.005f;

	gaussianBlurData_->stepWidth = 0.001f;
}

void GaussianBlur::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateGaussianBlurRes();
}


