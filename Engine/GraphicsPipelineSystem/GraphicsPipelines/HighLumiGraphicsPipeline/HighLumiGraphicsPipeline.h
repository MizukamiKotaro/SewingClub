#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class HighLumiGraphicsPipeline : public GraphicsPipeline
{
public:
	HighLumiGraphicsPipeline();
private:
	void InitializePSO() override;
};