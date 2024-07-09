#include "GraphicsPipelineFactory.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/ModelGraphicsPipline/ModelGraphicsPipline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/ParticleGraphicsPipeline/ParticleGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/SpriteGraphicsPipeline/SpriteGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/PointLightGraphicsPipline/PointLightGraphicsPipline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/SpotLightGraphicsPipline/SpotLightGraphicsPipline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/ContrastGraphicsPipeline/ContrastGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/HighLumiGraphicsPipeline/HighLumiGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/BlurGraphicsPipeline/BlurGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/GaussianBlurGraphicsPipeline/GaussianBlurGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/ScanNoiseGraphicsPipeline/ScanNoiseGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/NegaPosiInverseGraphicsPipeline/NegaPosiInverseGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/MosaicGraphicsPipeline/MosaicGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/RGBShiftGraphicsPipeline/RGBShiftGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/InstancingModelGraphicsPipline/InstancingModelGraphicsPipline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/NoiseGraphicsPipeline/NoiseGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/WaterOutlineGraphicsPipeline/WaterOutlineGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/CopyGraphicsPipeline/CopyGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/VignetteGraphicsPipeline/VignetteGraphicsPipeline.h"
#include "GraphicsPipelineSystem/GraphicsPipelines/DissolveGraphicsPipeline/DissolveGraphicsPipeline.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"

GraphicsPipeline* GraphicsPipelineFactory::CreateGraphicsPipeline(PipelineType pipelineType)
{
	GraphicsPipeline* graphicsPipeline = nullptr;

	switch (pipelineType)
	{
	case PipelineType::SPRITE:
		graphicsPipeline = new SpriteGraphicsPipeline();
		break;
	case PipelineType::COPY:
		graphicsPipeline = new CopyGraphicsPipeline();
		break;
	case PipelineType::MODEL:
		graphicsPipeline = new ModelGraphicsPipline();
		break;
	case PipelineType::PARTICLE:
		graphicsPipeline = new ParticleGraphicsPipeline();
		break;
	case PipelineType::POINT_LIGHT:
		graphicsPipeline = new PointLightGraphicsPipline();
		break;
	case PipelineType::SPOT_LIGHT:
		graphicsPipeline = new SpotLightGraphicsPipline();
		break;
	case PipelineType::CONTRAST:
		graphicsPipeline = new ContrastGraphicsPipeline();
		break;
	case PipelineType::HIGH_LUMI:
		graphicsPipeline = new HighLumiGraphicsPipeline();
		break;
	case PipelineType::BLUR:
		graphicsPipeline = new BlurGraphicsPipeline();
		break;
	case PipelineType::GAUSSIAN_BLUR:
		graphicsPipeline = new GaussianBlurGraphicsPipeline();
		break;
	case PipelineType::SCAN_NOISE:
		graphicsPipeline = new ScanNoiseGraphicsPipeline();
		break;
	case PipelineType::MOSAIC:
		graphicsPipeline = new MosaicGraphicsPipeline();
		break;
	case PipelineType::NEGA_POSI_INVERSE:
		graphicsPipeline = new NegaPosiInverseGraphicsPipeline();
		break;
	case PipelineType::RGB_SHIFT:
		graphicsPipeline = new RGBShiftGraphicsPipeline();
		break;
	case PipelineType::INSTANCING_MODEL:
		graphicsPipeline = new InstancingModelGraphicsPipline();
		break;
	case PipelineType::NOISE:
		graphicsPipeline = new NoiseGraphicsPipeline();
		break;
	case PipelineType::WATER_OUTLINE:
		graphicsPipeline = new WaterOutlineGraphicsPipeline();
		break;
	case PipelineType::VIGNETTE:
		graphicsPipeline = new VignetteGraphicsPipeline();
		break;
	case PipelineType::DISSOLVE:
		graphicsPipeline = new DissolveGraphicsPipeline();
		break;
	default:
		break;
	}
	
	return graphicsPipeline;
}