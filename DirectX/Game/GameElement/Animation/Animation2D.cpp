#include "Animation2D.h"
#include "Input/Input.h"
#include "ImGuiManager/ImGuiManager.h"
#include "GlobalVariables/GlobalVariables.h"

void Animation2D::Initialize(Model* model, const uint32_t& hDivNum, const uint32_t& wDivNum) {
	model_ = model;
	texParam_.divisionNumber = Vector2(static_cast<float>(wDivNum), static_cast<float>(hDivNum));

	uvScale_ = Vector3(1.0f / static_cast<float>(wDivNum), 1.0f / static_cast<float>(hDivNum),1.0f);

	uint32_t handle = hDivNum * wDivNum;
	sceneNumberList_.resize(handle);
	SceneEntry();
}

void Animation2D::Update() {
	if (Input::GetInstance()->PressedKey(DIK_SPACE)) {
		nowScene_ += 1u;
		if (nowScene_ >= sceneNumberList_.size()) { 
			nowScene_ = 0u;
		}
	}


	// UV座標の更新
	model_->SetUVParam(uvScale_, Vector3(0.0f, 0.0f, 0.0f), sceneNumberList_.at(nowScene_));
}

void Animation2D::SceneEntry() {
	uint32_t handle = 0u;
	for (uint32_t y = 0u; y < texParam_.divisionNumber.y; y++) {
		for (uint32_t x = 0u; x < texParam_.divisionNumber.x; x++) {
			Vector3 pos{};
			pos.x = static_cast<float>(x) * uvScale_.x;
			pos.y = static_cast<float>(y) * uvScale_.y;
			pos.z = 0.0f;
			sceneNumberList_[handle] = pos;
			handle++;
		}
	}
}
