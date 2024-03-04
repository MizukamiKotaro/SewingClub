#include "GraphicsPiplineManager.h"
#include "GraphicsPipelineSystem/BlendModeConfig.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "GraphicsPipelineSystem/GraphicsPipelineFactory/GraphicsPipelineFactory.h"
#include "GraphicsPipelineSystem/GraphicsPipeline/GraphicsPipeline.h"

GraphicsPipelineManager* GraphicsPipelineManager::GetInstance()
{
	static GraphicsPipelineManager instance;
	return &instance;
}

void GraphicsPipelineManager::Initialize()
{
	pipelineFactory_ = std::make_unique<GraphicsPipelineFactory>();
	currentPiplineType_ = PipelineType::SPRITE;

	pipelineMap_[currentPiplineType_].reset(pipelineFactory_->CreateGraphicsPipeline(currentPiplineType_));
	pipelineMap_[currentPiplineType_]->PreDraw();
}

void GraphicsPipelineManager::PreDraw()
{
	currentPiplineType_ = PipelineType::SPRITE;
	pipelineMap_[currentPiplineType_]->PreDraw();
}

void GraphicsPipelineManager::PreDraw(PipelineType type)
{
	if (currentPiplineType_ != type) {
		currentPiplineType_ = type;
		
		if (pipelineMap_.find(type) == pipelineMap_.end()) {
			pipelineMap_[currentPiplineType_].reset(pipelineFactory_->CreateGraphicsPipeline(currentPiplineType_));
		}
		pipelineMap_[type]->PreDraw();
	}
}

void GraphicsPipelineManager::SetBlendMode(PipelineType type, BlendMode blendMode)
{
	pipelineMap_[type]->SetBlendMode(blendMode);
}