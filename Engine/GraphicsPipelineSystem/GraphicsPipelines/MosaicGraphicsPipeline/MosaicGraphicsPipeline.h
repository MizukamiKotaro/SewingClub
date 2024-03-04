#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class MosaicGraphicsPipeline : public GraphicsPipeline
{
public:
	MosaicGraphicsPipeline();
private:
	void InitializePSO() override;
};