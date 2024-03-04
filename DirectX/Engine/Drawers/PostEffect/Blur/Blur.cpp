#include "Blur.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

Blur::Blur()
{
	piplineType_ = PipelineType::BLUR;
	blurData_ = nullptr;
	CreatePostEffect();
}

Blur::~Blur()
{
	blurResource_->Release();
}

void Blur::Draw(BlendMode blendMode)
{
	if (blurData_->pickRange <= 0.0f) {
		blurData_->pickRange = 0.01f;
	}
	if (blurData_->stepWidth <= 0.0f) {
		blurData_->stepWidth = 0.001f;
	}

	if (blendMode == BlendMode::kBlendModeNormal) {
		blurData_->isNormal = 1;
	}
	else {
		blurData_->isNormal = 0;
	}

	if (isInvisible_) {
		return;
	}

	materialData_->color = color_;

	PreDraw();

	transformData_->WVP = worldMat_ * Camera::GetOrthographicMat();
	materialData_->uvTransform = Matrix4x4::MakeAffinMatrix({ uvScale_.x,uvScale_.y,0.0f }, { 0.0f,0.0f,uvRotate_ }, { uvTranslate_.x,uvTranslate_.y,0.0f });

	psoManager_->SetBlendMode(piplineType_, blendMode);

	ID3D12GraphicsCommandList* commandList = DirectXBase::GetInstance()->GetCommandList();

	//Spriteの描画。変更に必要なものだけ変更する
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformResource_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(2, srvHandles_->gpuHandle);

	commandList->SetGraphicsRootConstantBufferView(3, blurResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(6, 1, 0, 0);

}

void Blur::CreateBlurRes()
{
	blurResource_ = DirectXBase::CreateBufferResource(sizeof(BlurData));

	blurResource_->Map(0, nullptr, reinterpret_cast<void**>(&blurData_));

	blurData_->angle = 0.0f;

	blurData_->pickRange = 0.06f;

	blurData_->stepWidth = 0.005f;

	blurData_->isCenterBlur = 1;

	blurData_->isNormal = 1;
}

void Blur::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateBlurRes();
}
