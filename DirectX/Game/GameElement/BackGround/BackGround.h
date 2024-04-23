#pragma once
#include "Sprite.h"
#include <memory>
#include "Camera/Camera.h"
#include "Star.h"
#include <array>

class BackGround {
public:
	BackGround();
	~BackGround() = default;

	void Update(Camera* camera);
	void Draw();

	static void StaticInitialize();
private:
	std::unique_ptr<Sprite> back_;
	std::unique_ptr<Camera> camera_; // 固定カメラ
	std::array <std::unique_ptr<Star>, 20u> starlist_;

	static class InstancingModelManager* instancingManager_;
	static const class InstancingMeshTexData* modelData_;

};
