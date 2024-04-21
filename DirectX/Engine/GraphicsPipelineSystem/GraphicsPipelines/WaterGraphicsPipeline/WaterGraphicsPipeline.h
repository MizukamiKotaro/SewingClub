#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class WaterGraphicsPipeline : public GraphicsPipeline
{
public:
	WaterGraphicsPipeline();
private:
	void InitializePSO() override;
};