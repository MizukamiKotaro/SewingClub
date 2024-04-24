#include "ScanNoise.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

ScanNoise::ScanNoise()
{
	piplineType_ = PipelineType::SCAN_NOISE;

	scanNoiseData_ = nullptr;

	CreatePostEffect();
}

ScanNoise::~ScanNoise()
{
	scanNoiseResource_->Release();
}

void ScanNoise::Draw(BlendMode blendMode)
{
	if (scanNoiseData_->power == 0.0f) {
		scanNoiseData_->power = 0.1f;
	}

	if (blendMode == BlendMode::kBlendModeNormal) {
		scanNoiseData_->isNormal = 1;
	}
	else {
		scanNoiseData_->isNormal = 0;
	}

	materialData_->color = color_;

	PreDraw();

	psoManager_->SetBlendMode(piplineType_, blendMode);

	//Spriteの描画。変更に必要なものだけ変更する
	//マテリアルCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	commandList_->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);

	commandList_->SetGraphicsRootConstantBufferView(2, scanNoiseResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList_->DrawInstanced(3, 1, 0, 0);
}

void ScanNoise::CreateScanNoiseRes()
{
	scanNoiseResource_ = DirectXBase::CreateBufferResource(sizeof(ScanNoiseData));
	scanNoiseResource_->Map(0, nullptr, reinterpret_cast<void**>(&scanNoiseData_));

	scanNoiseData_->minY = 0.2f;
	scanNoiseData_->width = 0.3f;
	scanNoiseData_->power = 0.1f;
	scanNoiseData_->isNormal = 1;
}

void ScanNoise::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateScanNoiseRes();
}