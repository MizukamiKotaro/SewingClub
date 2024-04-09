#include "ParticleDrawer.h"
#include "DirectXBase/DirectXBase.h"
#include "Camera.h"
#include <algorithm>
#include "Light/Light.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "DescriptorHeapManager/DescriptorHeapManager.h"
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"

#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "ILight/ILight.h"

const PipelineType ParticleDrawer::pipelineType_ = PipelineType::PARTICLE;

ParticleDrawer::ParticleDrawer(const ParticleMeshTexData& data)
{
	data_ = data;

	materialResource_ = DirectXBase::CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	*materialData_ = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) , 2 };
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();

	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	instancingResource_ = DirectXBase::CreateBufferResource(sizeof(ParticleForGPU) * kNumInstance);
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	for (uint32_t index = 0; index < kNumInstance; index++) {
		instancingData_[index].WVP = Matrix4x4::MakeIdentity4x4();
		instancingData_[index].World = Matrix4x4::MakeIdentity4x4();
		instancingData_[index].color = { 1.0f,1.0f,1.0f,0.0f };
	}

	CreateSRV();

	light_.Initialize();
}

ParticleDrawer::~ParticleDrawer()
{
	instancingResource_->Release();
	materialResource_->Release();
}

void ParticleDrawer::Draw(const Camera& camera, std::list<ParticleData>& blocks, BlendMode blendMode)
{
	PreDraw();

	uint32_t instaceNum = static_cast<uint32_t>(blocks.size());
	instaceNum = std::clamp<uint32_t>(instaceNum, 0, kNumInstance);
	uint32_t index = 0;

	for (std::list<ParticleData>::iterator iter = blocks.begin(); iter != blocks.end(); iter++) {
		if (index >= kNumInstance) {
			break;
		}

		instancingData_[index].World = iter->matrix_;
		instancingData_[index].WVP = instancingData_[index].World * camera.GetViewProjection();
		instancingData_[index].color = iter->color_;

		index++;
	}
	psoManager_->SetBlendMode(pipelineType_, blendMode);
	//Spriteの描画。変更に必要なものだけ変更する
	commandList_->IASetVertexBuffers(0, 1, &data_.modelData_->mesh.vertexBufferView_); // VBVを設定
	//マテリアルCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	//commandList->SetGraphicsRootConstantBufferView(1, instancingResource_->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootDescriptorTable(1, srvHandles_->gpuHandle);
	//平行光源CBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(3, light_.GetDirectionalLightGPUVirtualAddress());
	commandList_->SetGraphicsRootDescriptorTable(2, data_.texture_->handles_->gpuHandle);
	//描画!!!!（DrawCall/ドローコール）
	commandList_->DrawInstanced(UINT(data_.modelData_->mesh.verteces.size()), instaceNum, 0, 0);
}

void ParticleDrawer::PreDraw()
{
	psoManager_->PreDraw(pipelineType_);
}

void ParticleDrawer::SetLight(const ILight* light)
{
	light_.SetLight(light);
}

void ParticleDrawer::CreateSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = kNumInstance;
	srvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

	srvHandles_ = DescriptorHeapManager::GetInstance()->GetSRVDescriptorHeap()->GetNewDescriptorHandles();

	DirectXBase::GetInstance()->GetDevice()->CreateShaderResourceView(instancingResource_.Get(), &srvDesc, srvHandles_->cpuHandle);
}
