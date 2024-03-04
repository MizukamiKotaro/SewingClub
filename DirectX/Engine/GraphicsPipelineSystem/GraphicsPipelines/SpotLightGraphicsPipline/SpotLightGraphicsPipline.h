#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class SpotLightGraphicsPipline : public GraphicsPipeline
{
public:
	SpotLightGraphicsPipline();
private:
	void InitializePSO() override;
};