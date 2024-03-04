#pragma once
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"

class SRVDescriptorHeap : public DescriptorHeap
{
public:
	SRVDescriptorHeap();
};

class RTVDescriptorHeap : public DescriptorHeap
{
public:
	RTVDescriptorHeap();
};

class DSVDescriptorHeap : public DescriptorHeap
{
public:
	DSVDescriptorHeap();
};