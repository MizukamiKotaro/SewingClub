#include "PointLight.h"
#include "DirectXBase/DirectXBase.h"
#include "ModelDataManager.h"
#include "Camera.h"
#include <numbers>
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "ModelData/ModelData.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"

const Matrix4x4 PointLight::scaleMat_ = Matrix4x4::MakeScaleMatrix({ 1000.0f,1000.0f,1000.0f });
const Matrix4x4 PointLight::scaleInverseMat_ = Matrix4x4::Inverse(PointLight::scaleMat_);
const PipelineType PointLight::pipelineType_ = PipelineType::POINT_LIGHT;
const ModelData* PointLight::modelData_ = nullptr;

PointLight::PointLight()
{
	lightType_ = LightType::kPointLight;

	resource_ = DirectXBase::CreateBufferResource(sizeof(PointLightData));
	//データを書き込む
	light_ = nullptr;
	//書き込むためのアドレスを取得
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&light_));
	//書き込んでいく
	light_->color = { 1.0f,1.0f,1.0f,1.0f };
	light_->position = { 0.0f,0.0f,0.0f };
	light_->intensity = 0.0f;
	light_->radius = 1.0f;
	light_->decay = 0.1f;

	CreateTransformationResource();

	isDraw_ = true;
}

PointLight::~PointLight()
{
	transformationResource_->Release();
}

void PointLight::StaticInitialize()
{
	modelData_ = modelDataManager_->LoadObj("Plane");
}

void PointLight::Update()
{
	if (light_->radius <= 0.0f) {
		light_->radius = 0.01f;
	}
	if (light_->decay <= 0.0f) {
		light_->decay = 0.01f;
	}
}

void PointLight::Draw(const Camera& camera, BlendMode blendMode)
{
	if (isDraw_) {
		psoManager_->PreDraw(pipelineType_);

		Matrix4x4 billboardMat{};

		billboardMat = camera.transform_.worldMat_;

		billboardMat.m[3][0] = 0.0f;
		billboardMat.m[3][1] = 0.0f;
		billboardMat.m[3][2] = 0.0f;

		Matrix4x4 translateMat = Matrix4x4::MakeTranslateMatrix(light_->position + Vector3{ 0.0f,0.0f,-0.1f } *billboardMat);

		transformationData_->World = scaleMat_ * billboardMat * translateMat;

		transformationData_->WVP = transformationData_->World * camera.GetViewProjection();

		transformationData_->WorldInverse = scaleInverseMat_ * billboardMat * translateMat;

		psoManager_->SetBlendMode(pipelineType_, blendMode);

		//Spriteの描画。変更に必要なものだけ変更する
		commandList_->IASetVertexBuffers(0, 1, &modelData_->mesh.vertexBufferView_); // VBVを設定

		//TransformationMatrixCBufferの場所を設定
		commandList_->SetGraphicsRootConstantBufferView(1, transformationResource_->GetGPUVirtualAddress());

		// カメラの設定
		commandList_->SetGraphicsRootConstantBufferView(2, camera.GetGPUVirtualAddress());
		// pointLight の設定
		commandList_->SetGraphicsRootConstantBufferView(0, resource_->GetGPUVirtualAddress());

		//描画!!!!（DrawCall/ドローコール）
		commandList_->DrawInstanced(UINT(modelData_->mesh.verteces.size()), 1, 0, 0);
	}
}

void PointLight::CreateTransformationResource()
{//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	transformationResource_ = DirectXBase::CreateBufferResource(sizeof(TransformationMatrix));
	transformationData_ = nullptr;
	transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData_));
	*transformationData_ = { Matrix4x4::MakeIdentity4x4() ,Matrix4x4::MakeIdentity4x4(), Matrix4x4::Inverse(Matrix4x4::MakeIdentity4x4()) };
}
