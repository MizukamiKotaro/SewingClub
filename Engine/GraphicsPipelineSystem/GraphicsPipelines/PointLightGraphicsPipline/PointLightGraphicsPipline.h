#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class PointLightGraphicsPipline : public GraphicsPipeline
{
public:
	PointLightGraphicsPipline();
private:
	void InitializePSO() override;
};