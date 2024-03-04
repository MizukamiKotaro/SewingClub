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

	if (isInvisible_) {
		return;
	}

	materialData_->color = color_;

	PreDraw();

	transformData_->WVP = worldMat_ * Camera::GetOrthographicMat();
	materialData_->uvTransform = Matrix4x4::MakeAffinMatrix({ uvScale_.x,uvScale_.y,0.0f }, { 0.0f,0.0f,uvRotate_ }, { uvTranslate_.x,uvTranslate_.y,0.0f });

	psoManager_->SetBlendMode(piplineType_, blendMode);

	//Spriteの描画。変更に必要なものだけ変更する
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	//マテリアルCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(1, transformResource_->GetGPUVirtualAddress());

	commandList_->SetGraphicsRootDescriptorTable(2, srvHandles_->gpuHandle);

	commandList_->SetGraphicsRootConstantBufferView(3, mosaicResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList_->DrawInstanced(6, 1, 0, 0);
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