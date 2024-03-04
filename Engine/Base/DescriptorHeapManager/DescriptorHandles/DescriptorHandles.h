#pragma once
#include <d3d12.h>

class DescriptorHandles
{
public:
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	size_t no;
};