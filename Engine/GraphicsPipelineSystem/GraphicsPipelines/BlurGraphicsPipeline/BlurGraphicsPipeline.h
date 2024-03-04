#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class BlurGraphicsPipeline : public GraphicsPipeline
{
public:
	BlurGraphicsPipeline();
private:
	void InitializePSO() override;
};