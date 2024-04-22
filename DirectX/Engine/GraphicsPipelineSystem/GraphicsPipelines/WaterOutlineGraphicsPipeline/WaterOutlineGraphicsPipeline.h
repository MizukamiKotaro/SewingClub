#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class WaterOutlineGraphicsPipeline : public GraphicsPipeline
{
public:
	WaterOutlineGraphicsPipeline();
private:
	void InitializePSO() override;
};