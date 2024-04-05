#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class NoiseGraphicsPipeline : public GraphicsPipeline
{
public:
	NoiseGraphicsPipeline();
private:
	void InitializePSO() override;
};