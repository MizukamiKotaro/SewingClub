#include "Texture.h"
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"

Texture::~Texture()
{
	resource_->Release();
	intermediateResource_->Release();
}