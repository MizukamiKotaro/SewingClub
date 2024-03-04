#include "Contrast.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

Contrast::Contrast()
{
	piplineType_ = PipelineType::CONTRAST;

	contrastData_ = nullptr;

	CreatePostEffect();
}

Contrast::~Contrast()
{
	contrastResource_->Release();
}

void Contrast::Draw(BlendMode blendMode)
{

	if (isInvisible_) {
		return;
	}

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

	commandList->SetGraphicsRootConstantBufferView(3, contrastResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(6, 1, 0, 0);

}

void Contrast::CreateContrastRes()
{
	contrastResource_ = DirectXBase::CreateBufferResource(sizeof(ContrastData));

	contrastResource_->Map(0, nullptr, reinterpret_cast<void**>(&contrastData_));

	contrastData_->brightness_ = 0.2f;

	contrastData_->contrast_ = 3.0f;
}

void Contrast::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateContrastRes();
}

