#pragma once
#include "DrawQuadrangle.h"
#include <memory>
#include <unordered_map>

class DrawQuadrangleManager : public IDrawer
{
public:
	static DrawQuadrangleManager* GetInstance();
	void Initialize();
	void Clear();
	void Draw(const Camera& camera, const Vector3& leftTop, const Vector3& rightTop, const Vector3& leftBottom, const Vector3& rightBottom, const Vector4& color = { 1.0f,1.0f,1.0f,1.0f }, const BlendMode& blendMode = BlendMode::kBlendModeNormal);

private:
	DrawQuadrangleManager() = default;
	~DrawQuadrangleManager() = default;
	DrawQuadrangleManager(const DrawQuadrangleManager&) = delete;
	const DrawQuadrangleManager& operator=(const DrawQuadrangleManager&) = delete;

	std::unordered_map<int32_t, std::unique_ptr<DrawQuadrangle>> quadrangleMap_;
	const Texture* texture_ = nullptr;
	int32_t drawNum_ = 0;
};

