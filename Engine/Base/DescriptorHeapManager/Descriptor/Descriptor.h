#pragma once
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"

class Descriptor
{
public:
	DescriptorHandles handles;
	bool isUse;
};