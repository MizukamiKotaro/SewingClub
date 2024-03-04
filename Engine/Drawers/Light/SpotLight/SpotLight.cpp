#include "SpotLight.h"
#include "DirectXBase/DirectXBase.h"
#include <algorithm>
#include "Camera.h"
#include "ModelDataManager.h"
#include "calc.h"
#include <numbers>
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "ModelData/ModelData.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"

const Matrix4x4 SpotLight::scaleMat_ = Matrix4x4::MakeScaleMatrix({ 1000.0f,1000.0f,1000.0f });
const Matrix4x4 SpotLight::scaleInverseMat_ = Matrix4x4::Inverse(SpotLight::scaleMat_);
const PipelineType SpotLight::piplineType_ = PipelineType::SPOT_LIGHT;
const ModelData* SpotLight::modelData_ = nullptr;

SpotLight::SpotLight()
{
	lightType_ = LightType::kSpotLight;

	resource_ = DirectXBase::CreateBufferResource(sizeof(SpotLightData));
	//データを書き込む
	light_ = nullptr;
	//書き込むためのアドレスを取得
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&light_));
	//書き込んでいく
	light_->color = { 1.0f,1.0f,1.0f,1.0f };
	light_->position = { 0.0f,0.0f,0.0f };
	light_->intensity = 0.0f;
	light_->direction = { 0.0f,-1.0f,0.0f };
	light_->distance = 5.0f;
	light_->decay = 0.1f;
	light_->cosAngle = 0.7f;
	light_->cosFalloffStart = 0.8f;

	CreateTransformationResource();

	isDraw_ = true;
}

SpotLight::~SpotLight()
{
	transformationResource_->Release();
}

void SpotLight::StaticInitialize()
{
	modelData_ = modelDataManager_->LoadObj("Plane");
}

void SpotLight::Update()
{
	light_->direction = light_->direction.Normalize();

	light_->cosAngle = std::clamp<float>(light_->cosAngle, -1.0f, 0.998f);
	light_->cosFalloffStart = std::clamp<float>(light_->cosFalloffStart, light_->cosAngle + 0.001f, 0.999f);

	if (light_->distance <= 0.0f) {
		light_->distance = 0.01f;
	}
}

void SpotLight::Draw(const Camera& camera, BlendMode blendMode)
{
	if (isDraw_) {
		psoManager_->PreDraw(piplineType_);

		Matrix4x4 billboardMat{};

		billboardMat = camera.transform_.worldMat_;

		billboardMat.m[3][0] = 0.0f;
		billboardMat.m[3][1] = 0.0f;
		billboardMat.m[3][2] = 0.0f;

		Vector3 project = Calc::Project(camera.transform_.worldPos_ - light_->position, light_->direction);

		Vector3 rotate = camera.transform_.worldPos_ - (light_->position + project);

		rotate = rotate.Normalize();

		if (rotate.z != -1.0f) {
			billboardMat = billboardMat * Matrix4x4::DirectionToDirection({ 0.0f,0.0f,-1.0f }, rotate);
		}

		Matrix4x4 translateMat = Matrix4x4::MakeTranslateMatrix(light_->position + Vector3{ 0.0f,0.0f,0.1f } *billboardMat);

		transformationData_->World = scaleMat_ * billboardMat * translateMat;

		transformationData_->WVP = transformationData_->World * camera.GetViewProjection();

		transformationData_->WorldInverse = scaleInverseMat_ * billboardMat * translateMat;

		psoManager_->SetBlendMode(piplineType_, blendMode);

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

void SpotLight::CreateTransformationResource()
{
	transformationResource_ = DirectXBase::CreateBufferResource(sizeof(TransformationMatrix));
	transformationData_ = nullptr;
	transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData_));
	*transformationData_ = { Matrix4x4::MakeIdentity4x4() ,Matrix4x4::MakeIdentity4x4(), Matrix4x4::Inverse(Matrix4x4::MakeIdentity4x4()) };
}
