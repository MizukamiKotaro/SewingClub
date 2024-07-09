#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class VignetteGraphicsPipeline : public GraphicsPipeline
{
public:
	VignetteGraphicsPipeline();
private:
	void InitializePSO() override;
};