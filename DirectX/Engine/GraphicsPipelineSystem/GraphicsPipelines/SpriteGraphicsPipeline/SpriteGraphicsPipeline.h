#pragma once
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

class SpriteGraphicsPipeline : public GraphicsPipeline
{
public:
	SpriteGraphicsPipeline();
private:
	void InitializePSO() override;
};