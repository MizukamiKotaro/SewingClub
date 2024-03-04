#include "DirectionalLight.h"
#include "DirectXBase/DirectXBase.h"

DirectionalLight::DirectionalLight()
{
	lightType_ = LightType::kDirectionalLight;

	//平行光源用のリソースを作る。
	resource_ = DirectXBase::CreateBufferResource(sizeof(DirectionalLightData));
	//データを書き込む
	light_ = nullptr;
	//書き込むためのアドレスを取得
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&light_));
	//書き込んでいく
	light_->color = { 1.0f,1.0f,1.0f,1.0f };
	light_->direction = { 0.0f,-1.0f,1.0f };
	light_->direction = light_->direction.Normalize();
	light_->intensity = 1.0f;
}

DirectionalLight::~DirectionalLight() {}

void DirectionalLight::Update()
{
	light_->direction = light_->direction.Normalize();
}