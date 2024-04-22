#include "WaterOutline.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "WindowsInfo/WindowsInfo.h"

WaterOutline::WaterOutline()
{
	piplineType_ = PipelineType::WATER_OUTLINE;
	waterData_ = nullptr;
	CreatePostEffect();
}

WaterOutline::~WaterOutline()
{
	waterResource_->Release();
}

void WaterOutline::Draw(BlendMode blendMode)
{

	if (blendMode == BlendMode::kBlendModeNormal) {
		waterData_->isNormal = 1;
	}
	else {
		waterData_->isNormal = 0;
	}

	if (isInvisible_) {
		return;
	}

	materialData_->color = color_;

	PreDraw();

	transformData_->WVP = worldMat_ * Camera::GetOrthographicMat();
	materialData_->uvTransform = Matrix4x4::MakeAffinMatrix({ uvScale_.x,uvScale_.y,0.0f }, Vector3{ 0.0f,0.0f,uvRotate_ }, { uvTranslate_.x,uvTranslate_.y,0.0f });

	psoManager_->SetBlendMode(piplineType_, blendMode);

	ID3D12GraphicsCommandList* commandList = DirectXBase::GetInstance()->GetCommandList();

	//Spriteの描画。変更に必要なものだけ変更する
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformResource_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(2, srvHandles_->gpuHandle);

	commandList->SetGraphicsRootConstantBufferView(3, waterResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(6, 1, 0, 0);

}

void WaterOutline::CreateWaterRes()
{
	waterResource_ = DirectXBase::CreateBufferResource(sizeof(WaterOutlineData));

	waterResource_->Map(0, nullptr, reinterpret_cast<void**>(&waterData_));

	waterData_->screenSize = WindowsInfo::GetInstance()->GetWindowSize();
	waterData_->outlinePix = 2;
	waterData_->isNormal = 1;
}

void WaterOutline::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateWaterRes();
}
