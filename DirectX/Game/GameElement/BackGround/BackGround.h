#pragma once
#include "Sprite.h"
#include <memory>
#include "Camera/Camera.h"
#include "Star.h"
#include <array>
#include "StageEditor/StageEditor.h"

class BackGround {
public:
	BackGround();
	~BackGround() = default;

	void Initialize();
	void Update(Camera* camera);
	void Draw();

private:
	static void StaticInitialize();

	// グローバル変数のセット
	void SetGlobalVariable();
	// グローバル変数の更新
	void ApplyGlobalVariable();

private:
	std::unique_ptr<Sprite> back_;
	Vector3 bgColor_;
	std::unique_ptr<Camera> camera_; // 固定カメラ
	std::array <std::unique_ptr<Star>, 20u> starlist_;

	static class InstancingModelManager* instancingManager_;
	static const class InstancingMeshTexData* modelData_;

	std::unique_ptr<StageEditor> stageEditor_;
	std::unique_ptr<GlobalVariableUser> global_;

};
