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

	PreDraw();

	psoManager_->SetBlendMode(piplineType_, blendMode);

	//Spriteの描画。変更に必要なものだけ変更する
	//マテリアルCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	commandList_->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);

	commandList_->SetGraphicsRootConstantBufferView(2, negaPosiInverseResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList_->DrawInstanced(3, 1, 0, 0);
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