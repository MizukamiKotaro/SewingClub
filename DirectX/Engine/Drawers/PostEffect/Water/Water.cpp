#include "Water.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "WindowsInfo/WindowsInfo.h"

Water::Water()
{
	piplineType_ = PipelineType::WATER;
	waterData_ = nullptr;
	CreatePostEffect();
}

Water::~Water()
{
	waterResource_->Release();
}

void Water::Initialize()
{
	waterData_->time = 0.0f;
}

void Water::Update(const float& time)
{
	waterData_->time += time;
}

void Water::Draw(BlendMode blendMode)
{
	if (waterData_->density <= 0.0f) {
		waterData_->density = 1.0f;
	}

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

void Water::CreateWaterRes()
{
	waterResource_ = DirectXBase::CreateBufferResource(sizeof(WaterData));

	waterResource_->Map(0, nullptr, reinterpret_cast<void**>(&waterData_));

	waterData_->density = 10.0f;
	waterData_->screenSize = WindowsInfo::GetInstance()->GetWindowSize();
	waterData_->time = 0.0f;
	waterData_->isNormal = 1;
}

void Water::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateWaterRes();
}
