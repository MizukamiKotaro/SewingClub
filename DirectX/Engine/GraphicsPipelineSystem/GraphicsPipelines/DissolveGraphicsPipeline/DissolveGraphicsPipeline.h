#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class DissolveGraphicsPipeline : public GraphicsPipeline
{
public:
	DissolveGraphicsPipeline();
private:
	void InitializePSO() override;
};