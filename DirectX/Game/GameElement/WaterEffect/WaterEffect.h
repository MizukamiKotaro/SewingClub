#pragma once
#include <memory>
#include "Noise/Noise.h"
#include "PostEffect/PostEffect.h"
#include "HighLumi/HighLumi.h"
#include "StageEditor/StageEditor.h"

class Camera;

class WaterEffect {
public:
	
	WaterEffect(const Vector3& cameraPos);

	void Initialize();

	void Update(const float& deltaTime);

	void Draw();

	void PreDrawBackGround();

	void PostDrawBackGround();

	void PreDrawWaterArea();

	void PostDrawWaterArea();

private:

	// グローバル変数のセット
	void SetGlobalVariable();
	// グローバル変数の更新
	void ApplyGlobalVariable();

private:
	std::unique_ptr<Noise> noise_;
	std::unique_ptr<PostEffect> post_;
	std::unique_ptr<HighLumi> highLumi_;

	std::unique_ptr<GlobalVariableUser> global_;
	std::unique_ptr<StageEditor> stageEditor_;
};