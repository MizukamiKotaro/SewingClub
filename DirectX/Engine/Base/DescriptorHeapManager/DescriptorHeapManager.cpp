#include "DescriptorHeapManager.h"
#include "DescriptorHeaps/DescriptorHeaps.h"

DescriptorHeapManager* DescriptorHeapManager::GetInstance()
{
	static DescriptorHeapManager instance;
	return &instance;
}

void DescriptorHeapManager::Initialize()
{
	srvHeap_ = std::make_unique<SRVDescriptorHeap>();
	rtvHeap_ = std::make_unique<RTVDescriptorHeap>();
	dsvHeap_ = std::make_unique<DSVDescriptorHeap>();
}

void DescriptorHeapManager::Finalize()
{
	srvHeap_.reset();
	rtvHeap_.reset();
	dsvHeap_.reset();
}
