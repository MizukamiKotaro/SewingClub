#include "Animation2D.h"
#include "Input/Input.h"
#include "ImGuiManager/ImGuiManager.h"
#include "GlobalVariables/GlobalVariables.h"

void Animation2D::Initialize(std::string fileName, const uint32_t& hDivNum, const uint32_t& wDivNum) {
	global_ = std::make_unique<GlobalVariableUser>(chunkName, fileName);
	// 新しくアニメーションを作る時のみ代入がされる。もともとある場合はファイル側からもらってくる
	texParam_.divisionNumber = Vector2(static_cast<float>(wDivNum), static_cast<float>(hDivNum));

	SetGlobalVariable();

	SceneEntry();
}

Transform Animation2D::GetSceneUV(const uint32_t& scene) {
	UpdateTrans(scene);
	return transform_;
}

void Animation2D::Play(bool flag) {
	if (isPlay_ == flag) { return; }
	isPlay_ = flag;
	nowFrame_ = 0.0f;
	nowScene_ = 0u;
}

bool Animation2D::Update() {
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG

	if (!isPlay_) { return false; }

	AnimationCount();
	uint32_t number = keyParam_.at(nowScene_).sceneNumber;
	// UV座標の更新
	UpdateTrans(number);
	return true;
}

void Animation2D::SceneEntry() {
	uint32_t handle = static_cast<uint32_t>(texParam_.divisionNumber.x) * static_cast<uint32_t>(texParam_.divisionNumber.y);
	sceneNumberList_.resize(handle);
	handle = 0u;
	for (uint32_t y = 0u; y < texParam_.divisionNumber.y; y++) {
		for (uint32_t x = 0u; x < texParam_.divisionNumber.x; x++) {
			Vector3 pos{};
			pos.x = static_cast<float>(x) * texParam_.uvScale.x;
			pos.y = static_cast<float>(y) * texParam_.uvScale.y;
			pos.z = 0.0f;
			sceneNumberList_[handle] = pos;
			handle++;
		}
	}
	// uvScaleの設定
	texParam_.uvScale = Vector3(1.0f / texParam_.divisionNumber.x, 1.0f / texParam_.divisionNumber.y, 1.0f);
}

bool Animation2D::AnimationCount() {
	if (keyParam_.at(nowScene_).keyFrame <= nowFrame_++) {
		nowScene_ += 1u;
		nowFrame_ = 0.0f;
		// 最後まで行った&&ループするならば
		if (nowScene_ == keyParam_.size() && isLoop_) {
			nowScene_ = 0u;
		}
		return true;
	}
	return false;
}

void Animation2D::SetGlobalVariable() {
	if (global_) {
		std::string initTree = "詳細設定";
		global_->AddItem("横分割数", static_cast<int>(texParam_.divisionNumber.x),initTree);
		global_->AddItem("縦分割数", static_cast<int>(texParam_.divisionNumber.y),initTree);
		global_->AddItem("キーの最大数", static_cast<int>(maxKeyNumber_), initTree);
		keyParam_.resize(maxKeyNumber_);
		for (uint32_t index = 0; index < keyParam_.size(); index++) {
			std::string tree = "Key" + std::to_string(index);
			int no = index / 10;
			no = no * 10;
			std::string tree1 = "Key" + std::to_string(no) + "～" + std::to_string(no + 9);
			global_->AddItem("画像No", static_cast<int>(keyParam_.at(index).sceneNumber), tree1, tree);
			global_->AddItem("切替わるまでのフレーム", keyParam_.at(index).keyFrame, tree1, tree);
		}
	}
	ApplyGlobalVariable();
}

void Animation2D::ApplyGlobalVariable() {
	if (global_) {
		std::string initTree = "詳細設定";
		maxKeyNumber_ = static_cast<uint32_t>(global_->GetIntValue("キーの最大数", initTree));

		// 分割数の代入
		texParam_.divisionNumber.y = static_cast<float>(global_->GetIntValue("縦分割数", initTree));
		texParam_.divisionNumber.x = static_cast<float>(global_->GetIntValue("横分割数", initTree));
		SceneEntry();
		
		// キーコンテナ以上に生成された場合リサイズする
		if (keyParam_.size() < maxKeyNumber_) {
			keyParam_.resize(maxKeyNumber_);
			// 近年稀にみるゴミです
			SetGlobalVariable();
		}
		
		for (uint32_t index = 0; index < keyParam_.size(); index++) {
			std::string tree = "Key" + std::to_string(index);
			int no = index / 10;
			no = no * 10;
			std::string tree1 = "Key" + std::to_string(no) + "～" + std::to_string(no + 9);
			keyParam_.at(index).sceneNumber = static_cast<uint32_t>(global_->GetIntValue("画像No", tree1, tree));
			keyParam_.at(index).keyFrame = global_->GetFloatValue("切替わるまでのフレーム", tree1, tree);
		}
		
	}


}

void Animation2D::UpdateTrans(const uint32_t& listNum) {
	transform_.scale_ = texParam_.uvScale;
	transform_.rotate_ = Vector3(0.0f, 0.0f, uvRotate_);
	transform_.translate_ = sceneNumberList_.at(listNum);
	transform_.UpdateMatrix();
}
