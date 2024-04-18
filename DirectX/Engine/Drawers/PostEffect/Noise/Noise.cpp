#include "Noise.h"

#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "WindowsInfo/WindowsInfo.h"

Noise::Noise()
{
	piplineType_ = PipelineType::NOISE;
	noiseData_ = nullptr;
	CreatePostEffect();
}

Noise::~Noise()
{
	noiseResource_->Release();
}

void Noise::Initialize()
{
	noiseData_->time = 0.0f;
}

void Noise::Update(const float& time)
{
	noiseData_->time += time;
}

void Noise::Draw(BlendMode blendMode)
{
	if (noiseData_->density <= 0.0f) {
		noiseData_->density = 1.0f;
	}

	if (blendMode == BlendMode::kBlendModeNormal) {
		noiseData_->isNormal = 1;
	}
	else {
		noiseData_->isNormal = 0;
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

	commandList->SetGraphicsRootConstantBufferView(3, noiseResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(6, 1, 0, 0);

}

void Noise::CreateNoiseRes()
{
	noiseResource_ = DirectXBase::CreateBufferResource(sizeof(NoiseData));

	noiseResource_->Map(0, nullptr, reinterpret_cast<void**>(&noiseData_));

	noiseData_->density = 10.0f;
	noiseData_->screenSize = WindowsInfo::GetInstance()->GetWindowSize();
	noiseData_->time = 0.0f;
	noiseData_->type = NoiseType::LIGHTNING;
	noiseData_->isNormal = 1;
}

void Noise::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateNoiseRes();
}
