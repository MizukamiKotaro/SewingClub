#include "Light.h"
#include "LightSingleton/LightSingleton.h"
#include "DirectionalLight/DirectionalLight.h"
#include "PointLight/PointLight.h"
#include "SpotLight/SpotLight.h"

const LightSingleton* Light::light_ = nullptr;

void Light::StaticInitialize()
{
	light_ = LightSingleton::GetInstance();
}

void Light::Initialize()
{
	directionalLight_ = light_->GetDirectionaLight();
	pointLight_ = light_->GetPointLight();
	spotLight_ = light_->GetSpotLight();
}

void Light::SetLight(const ILight* light)
{
	switch (light->GetLightType())
	{
	case LightType::kDirectionalLight:
		directionalLight_ = light;
		break;
	case LightType::kPointLight:
		pointLight_ = light;
		break;
	case LightType::kSpotLight:
		spotLight_ = light;
		break;
	default:
		break;
	}
}

const D3D12_GPU_VIRTUAL_ADDRESS Light::GetDirectionalLightGPUVirtualAddress() const
{
	return directionalLight_->GetGPUVirtualAddress();
}

const D3D12_GPU_VIRTUAL_ADDRESS Light::GetPointLightGPUVirtualAddress() const
{
	return pointLight_->GetGPUVirtualAddress();
}

const D3D12_GPU_VIRTUAL_ADDRESS Light::GetSpotLightGPUVirtualAddress() const
{
	return spotLight_->GetGPUVirtualAddress();
}