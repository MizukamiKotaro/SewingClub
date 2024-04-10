#include "Animation2D.h"
#include "Input/Input.h"

void Animation2D::Initialize(Model* model, const float& height, const float& width, const uint32_t& hDivNum, const uint32_t& wDivNum) {
	model_ = model;
	texParam_.textureSize = Vector2(width, height);
	texParam_.divisionNumber = Vector2(static_cast<float>(wDivNum), static_cast<float>(hDivNum));
	texParam_.sceneSize = Vector2(static_cast<float>(wDivNum) / width, static_cast<float>(hDivNum) / height);

	uvScale_ = Vector3(1.0f / static_cast<float>(wDivNum), 1.0f / static_cast<float>(hDivNum),1.0f);

	uint32_t handle = hDivNum * wDivNum;
	sceneNumberList_.resize(handle);
	SceneEntry();
}

void Animation2D::Update() {
	if (Input::GetInstance()->PressedKey(DIK_SPACE)) {
		nowScene_ += 1u;
		if (nowScene_ >= sceneNumberList_.size()) { nowScene_ = 0u; }
	}
	model_->SetUVParam(uvScale_, Vector3(0.0f, 0.0f, 0.0f), sceneNumberList_.at(nowScene_));
}

void Animation2D::SceneEntry() {
	for (uint32_t y = 0u; y < texParam_.divisionNumber.y; y++) {
		for (uint32_t x = 0u; x < texParam_.divisionNumber.x; x++) {
			Vector3 pos{};
			pos.x = static_cast<float>(x) * texParam_.sceneSize.x;
			pos.y = static_cast<float>(y) * texParam_.sceneSize.y;
			pos.z = 0.0f;
			uint32_t handle = x + y;
			sceneNumberList_[handle] = pos;
		}
	}
}
