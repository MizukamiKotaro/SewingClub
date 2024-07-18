#pragma once
#include <memory>
#include "Noise/Noise.h"
#include "GlobalVariables/GlobalVariableUser.h"

class WaterSurface {
public:
	
	WaterSurface();

	void Initialize();

	void Update(const float& deltaTime);

	void Draw();

	void PreDraw();

	void PostDraw();

private:
	// グローバル変数のセット
	void SetGlobalVariable();
	// グローバル変数の更新
	void ApplyGlobalVariable();

private:
	float add_;
	std::unique_ptr<Noise> noise_;
	std::unique_ptr<GlobalVariableUser> global_;
};