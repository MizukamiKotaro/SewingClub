#pragma once
#include <d3d12.h>

class LightSingleton;
class ILight;

// モデルなどの変数に使う用
class Light 
{
public:
	static void StaticInitialize();

	void Initialize();

	void SetLight(const ILight* light);

	const D3D12_GPU_VIRTUAL_ADDRESS GetDirectionalLightGPUVirtualAddress() const;
	const D3D12_GPU_VIRTUAL_ADDRESS GetPointLightGPUVirtualAddress() const;
	const D3D12_GPU_VIRTUAL_ADDRESS GetSpotLightGPUVirtualAddress() const;

private:
	static const LightSingleton* light_;

	const ILight* directionalLight_ = nullptr;
	const ILight* pointLight_ = nullptr;
	const ILight* spotLight_ = nullptr;
};