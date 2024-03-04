#include "DescriptorHeaps.h"
#include "Engine/Base/DirectXBase/DirectXBase.h"

const size_t kNumDescriptors_ = 256;

SRVDescriptorHeap::SRVDescriptorHeap()
{
	heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptors_.clear();
	heap_ = DescriptorHeap::CreateDescriptorHeap(DirectXBase::GetInstance()->GetDevice(), heapType_, kNumDescriptors_, true);
}

RTVDescriptorHeap::RTVDescriptorHeap()
{
	heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptors_.clear();
	heap_ = DescriptorHeap::CreateDescriptorHeap(DirectXBase::GetInstance()->GetDevice(), heapType_, kNumDescriptors_, false);
}

DSVDescriptorHeap::DSVDescriptorHeap()
{
	heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descriptors_.clear();
	heap_ = DescriptorHeap::CreateDescriptorHeap(DirectXBase::GetInstance()->GetDevice(), heapType_, kNumDescriptors_, false);
}