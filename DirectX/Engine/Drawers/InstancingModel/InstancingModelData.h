#pragma once
#include "Matrix4x4.h"
#include "Vector4.h"
#include "ModelData/ModelData.h"
#include "GraphicsPipelineSystem/BlendModeConfig.h"

class InstancingModelData {
public:
	Matrix4x4 matrix_;
	Matrix4x4 uvMatrix_;
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
};

class InstancingMeshTexData {
public:
	const ModelData* modelData_;
	const Texture* texture_;
	BlendMode blendMode_;
};