#include "Mosaic.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "WindowsInfo/WindowsInfo.h"

Mosaic::Mosaic()
{
	piplineType_ = PipelineType::MOSAIC;

	mosaicData_ = nullptr;

	CreatePostEffect();
}

Mosaic::~Mosaic()
{
	mosaicResource_->Release();
}

void Mosaic::Draw(BlendMode blendMode)
{
	if (blendMode == BlendMode::kBlendModeNormal) {
		mosaicData_->isNormal = 1;
	}
	else {
		mosaicData_->isNormal = 0;
	}

	materialData_->color = color_;

	PreDraw();

	psoManager_->SetBlendMode(piplineType_, blendMode);

	//Spriteの描画。変更に必要なものだけ変更する
	//マテリアルCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	commandList_->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);

	commandList_->SetGraphicsRootConstantBufferView(2, mosaicResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList_->DrawInstanced(3, 1, 0, 0);
}

void Mosaic::CreateMosaicRes()
{
	mosaicResource_ = DirectXBase::CreateBufferResource(sizeof(MosaicData));
	mosaicResource_->Map(0, nullptr, reinterpret_cast<void**>(&mosaicData_));

	mosaicData_->density = 30.0f;
	mosaicData_->isSquare = 1;
	mosaicData_->screenSize = WindowsInfo::GetInstance()->GetWindowSize();
	mosaicData_->isNormal = 1;
}

void Mosaic::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateMosaicRes();
}