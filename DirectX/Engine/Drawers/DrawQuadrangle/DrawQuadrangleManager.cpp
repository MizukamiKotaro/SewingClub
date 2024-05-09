#include "DrawQuadrangleManager.h"

#include "TextureManager/TextureManager.h"
#include "Camera.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"

DrawQuadrangleManager* DrawQuadrangleManager::GetInstance()
{
	static DrawQuadrangleManager instance;
	return &instance;
}

void DrawQuadrangleManager::Initialize()
{
	texture_ = textureManager_->LoadTexture("white.png");
	drawNum_ = 0;
	quadrangleMap_.clear();
	for (int32_t i = 0; i < 50; i++) {
		quadrangleMap_[i] = std::make_unique<DrawQuadrangle>();
	}
}

void DrawQuadrangleManager::Clear()
{
	drawNum_ = 0;
}

void DrawQuadrangleManager::Draw(const Camera& camera, const Vector3& leftTop, const Vector3& rightTop, const Vector3& leftBottom, const Vector3& rightBottom, const Vector4& color, const BlendMode& blendMode)
{
	if (quadrangleMap_.find(drawNum_) == quadrangleMap_.end()) {
		quadrangleMap_[drawNum_] = std::make_unique<DrawQuadrangle>();
	}
	Matrix4x4 wvp = Matrix4x4::MakeIdentity4x4() * camera.GetViewProjection();
	Vector3 topLeft = leftTop * wvp;
	topLeft = topLeft * camera.GetViewportMat();
	Vector3 topRight = rightTop * wvp;
	topRight = topRight * camera.GetViewportMat();
	Vector3 bottomLeft = leftBottom * wvp;
	bottomLeft = bottomLeft * camera.GetViewportMat();
	Vector3 bottomRight = rightBottom * wvp;
	bottomRight = bottomRight * camera.GetViewportMat();

	quadrangleMap_[drawNum_]->SetVertex(topLeft, topRight, bottomLeft, bottomRight);
	quadrangleMap_[drawNum_]->Draw(camera, color, *texture_, blendMode);
	drawNum_++;
}

