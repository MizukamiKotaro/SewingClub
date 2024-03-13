#include "NegaPosiInverse.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

NegaPosiInverse::NegaPosiInverse()
{
	piplineType_ = PipelineType::NEGA_POSI_INVERSE;

	negaPosiInverseData_ = nullptr;

	CreatePostEffect();
}

NegaPosiInverse::~NegaPosiInverse()
{
	negaPosiInverseResource_->Release();
}

void NegaPosiInverse::Draw(BlendMode blendMode)
{
	if (blendMode == BlendMode::kBlendModeNormal) {
		negaPosiInverseData_->isNormal = 1;
	}
	else {
		negaPosiInverseData_->isNormal = 0;
	}

	if (isInvisible_) {
		return;
	}

	PreDraw();

	transformData_->WVP = worldMat_ * Camera::GetOrthographicMat();
	materialData_->uvTransform = Matrix4x4::MakeAffinMatrix({ uvScale_.x,uvScale_.y,0.0f }, Vector3{ 0.0f,0.0f,uvRotate_ }, { uvTranslate_.x,uvTranslate_.y,0.0f });

	psoManager_->SetBlendMode(piplineType_, blendMode);

	//Spriteの描画。変更に必要なものだけ変更する
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	//マテリアルCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(1, transformResource_->GetGPUVirtualAddress());

	commandList_->SetGraphicsRootDescriptorTable(2, srvHandles_->gpuHandle);

	commandList_->SetGraphicsRootConstantBufferView(3, negaPosiInverseResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList_->DrawInstanced(6, 1, 0, 0);
}

void NegaPosiInverse::CreateNegaPosiInverseRes()
{
	negaPosiInverseResource_ = DirectXBase::CreateBufferResource(sizeof(NegaPosiInverseData));
	negaPosiInverseResource_->Map(0, nullptr, reinterpret_cast<void**>(&negaPosiInverseData_));

	negaPosiInverseData_->isNormal = 1;
}

void NegaPosiInverse::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateNegaPosiInverseRes();
}