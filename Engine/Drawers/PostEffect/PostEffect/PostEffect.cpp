#include "PostEffect.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"

PostEffect::PostEffect()
{
	piplineType_ = PipelineType::SPRITE;

	CreatePostEffect();
}