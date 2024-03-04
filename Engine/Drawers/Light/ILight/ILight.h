#pragma once
#include <wrl.h>
#include <d3d12.h>

enum class LightType {
	kDirectionalLight,
	kPointLight,
	kSpotLight
};

class ILight
{
public:
	virtual ~ILight();
	virtual void Update() = 0;
	const D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return resource_->GetGPUVirtualAddress(); }
	const LightType GetLightType() const { return lightType_; }
protected:
	LightType lightType_;
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
};