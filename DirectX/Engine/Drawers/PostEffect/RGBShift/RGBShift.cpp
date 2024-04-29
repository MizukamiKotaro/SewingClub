#include "RGBShift.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

RGBShift::RGBShift()
{
	piplineType_ = PipelineType::RGB_SHIFT;

	rgbShiftData_ = nullptr;

	CreatePostEffect();
}

RGBShift::~RGBShift()
{
	rgbShiftResource_->Release();
}

void RGBShift::Draw(BlendMode blendMode)
{
	if (blendMode == BlendMode::kBlendModeNormal) {
		rgbShiftData_->isNormal = 1;
	}
	else {
		rgbShiftData_->isNormal = 0;
	}

	materialData_->color = color_;

	PreDraw();

	psoManager_->SetBlendMode(piplineType_, blendMode);

	//Spriteの描画。変更に必要なものだけ変更する
	//マテリアルCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	commandList_->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);

	commandList_->SetGraphicsRootConstantBufferView(2, rgbShiftResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList_->DrawInstanced(3, 1, 0, 0);
}

void RGBShift::CreateRGBShiftRes()
{
	rgbShiftResource_ = DirectXBase::CreateBufferResource(sizeof(RGBShiftData));
	rgbShiftResource_->Map(0, nullptr, reinterpret_cast<void**>(&rgbShiftData_));

	rgbShiftData_->shift = 0.007f;
	rgbShiftData_->isNormal = 1;
}

void RGBShift::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateRGBShiftRes();
}