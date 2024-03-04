#pragma once

class GraphicsPipeline;
enum class PipelineType;

class IGraphicsPipelineFactory
{
public:
	virtual ~IGraphicsPipelineFactory() = default;

	virtual GraphicsPipeline* CreateGraphicsPipeline(PipelineType pipelineType) = 0;
};