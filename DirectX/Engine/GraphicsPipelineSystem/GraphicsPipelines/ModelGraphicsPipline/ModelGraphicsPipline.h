#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class ModelGraphicsPipline : public GraphicsPipeline
{
public:
	ModelGraphicsPipline();
private:
	void InitializePSO() override;
};