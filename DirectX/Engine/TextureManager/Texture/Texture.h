#pragma once
#include <wrl.h>
#include <d3d12.h>

class DescriptorHandles;

class Texture {
public:
	~Texture();
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource_;
	const DescriptorHandles* handles_;
};