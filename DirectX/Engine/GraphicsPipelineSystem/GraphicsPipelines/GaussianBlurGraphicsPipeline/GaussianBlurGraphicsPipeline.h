#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class GaussianBlurGraphicsPipeline : public GraphicsPipeline
{
public:
	GaussianBlurGraphicsPipeline();
private:
	void InitializePSO() override;
};