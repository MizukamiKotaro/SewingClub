#pragma once
#include <d3d12.h>

enum class PipelineType;
class GraphicsPipelineManager;
class TextureManager;
class ModelDataManager;

class IDrawer {
public:
	static void StaticInitialize();
protected:
	static ID3D12GraphicsCommandList* commandList_;
	static GraphicsPipelineManager* psoManager_;
	static TextureManager* textureManager_;
	static ModelDataManager* modelDataManager_;
};