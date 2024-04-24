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
	camera = nullptr;
}

Noise::~Noise()
{
	noiseResource_->Release();
}

void Noise::SetCameraPos(const Vector3& pos)
{
	camera = &pos;
	noiseData_->cameraPos = Vector2{ camera->x,camera->y };
}

void Noise::Initialize()
{
	noiseData_->time = 0.0f;
	if (camera) {
		noiseData_->cameraPos = Vector2{ camera->x,camera->y };
	}
}

void Noise::Update(const float& time)
{
	noiseData_->time += time;
	if (camera) {
		noiseData_->cameraPos = Vector2{ camera->x,camera->y };
	}
}

void Noise::Draw(BlendMode blendMode)
{
	if (noiseData_->density <= 0.0f) {
		noiseData_->density = 0.1f;
	}

	if (noiseData_->moveScale <= 0.0f) {
		noiseData_->moveScale = 0.1f;
	}

	if (blendMode == BlendMode::kBlendModeNormal) {
		noiseData_->isNormal = 1;
	}
	else {
		noiseData_->isNormal = 0;
	}

	materialData_->color = color_;

	PreDraw();

	psoManager_->SetBlendMode(piplineType_, blendMode);

	ID3D12GraphicsCommandList* commandList = DirectXBase::GetInstance()->GetCommandList();

	//Spriteの描画。変更に必要なものだけ変更する
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);

	commandList->SetGraphicsRootConstantBufferView(2, noiseResource_->GetGPUVirtualAddress());

	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(3, 1, 0, 0);

}

void Noise::CreateNoiseRes()
{
	noiseResource_ = DirectXBase::CreateBufferResource(sizeof(NoiseData));

	noiseResource_->Map(0, nullptr, reinterpret_cast<void**>(&noiseData_));

	noiseData_->density = 10.0f;
	noiseData_->screenSize = WindowsInfo::GetInstance()->GetWindowSize();
	noiseData_->time = 0.0f;
	noiseData_->lightningColor = { 0.8f,0.8f,0.8f,1.0f };
	noiseData_->waterColor = { 0.3f,1.0f,0.8f,1.0f };
	noiseData_->type = NoiseType::WATER;
	noiseData_->isNormal = 1;
	noiseData_->cameraPos = {};
	noiseData_->moveScale = 2.0f;
}

void Noise::CreateResources()
{
	BasePostEffect::CreateResources();

	CreateNoiseRes();
}
