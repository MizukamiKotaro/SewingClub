#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class CopyGraphicsPipeline : public GraphicsPipeline
{
public:
	CopyGraphicsPipeline();
private:
	void InitializePSO() override;
};