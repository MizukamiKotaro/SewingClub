#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class InstancingModelGraphicsPipline : public GraphicsPipeline
{
public:
	InstancingModelGraphicsPipline();
private:
	void InitializePSO() override;
};