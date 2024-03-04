#pragma once
#include "GraphicsPipelineSystem/IGraphicsPipelineFactory/IGraphicsPipelineFactory.h"

class GraphicsPipelineFactory : public IGraphicsPipelineFactory
{
public:
	GraphicsPipeline* CreateGraphicsPipeline(PipelineType pipelineType) override;
};
