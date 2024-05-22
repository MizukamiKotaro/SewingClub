#include "Animation2D.h"
#include "Input/Input.h"
#include "ImGuiManager/ImGuiManager.h"
#include "GlobalVariables/GlobalVariables.h"
#include "AnimationManager.h"

void Animation2DData::Initialize(std::string fileName, const uint32_t& hDivNum, const uint32_t& wDivNum) {
	global_ = std::make_unique<GlobalVariableUser>(chunkName, fileName);
	// 新しくアニメーションを作る時のみ代入がされる。もともとある場合はファイル側からもらってくる
	texParam_.divisionNumber = Vector2(static_cast<float>(wDivNum), static_cast<float>(hDivNum));

	SetGlobalVariable();
	SceneEntry();
}

void Animation2DData::Initialize(const uint32_t& hDivNum, const uint32_t& wDivNum) {
	texParam_.divisionNumber = Vector2(static_cast<float>(wDivNum), static_cast<float>(hDivNum));
	SceneEntry();
}

void Animation2DData::SceneEntry() {
	uint32_t handle = static_cast<uint32_t>(texParam_.divisionNumber.x) * static_cast<uint32_t>(texParam_.divisionNumber.y);
	sceneNumberList_.resize(handle);
	texParam_.uvScale = Vector3(1.0f / texParam_.divisionNumber.x, 1.0f / texParam_.divisionNumber.y, 1.0f);
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
}

void Animation2DData::SetGlobalVariable() {
	if (global_) {
		std::string initTree = "詳細設定";
		global_->AddItem("横分割数", static_cast<int>(texParam_.divisionNumber.x), initTree);
		global_->AddItem("縦分割数", static_cast<int>(texParam_.divisionNumber.y), initTree);
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

void Animation2DData::ApplyGlobalVariable() {
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

Animation2D::Animation2D(Animation2DData* data) {
	data_ = data;
}

bool Animation2D::Update(std::string path) {
#ifdef _DEBUG
	data_->ApplyGlobalVariable();
#endif // _DEBUG

	if (!isPlay_) { return false; }
	if (path != oldPath_) { 
		data_ = AnimationManager::GetInstance()->AddAnimation(path);
		nowScene_ = 0u;
		nowFrame_ = 0.0f;
	}

	AnimationCount();
	uint32_t number = data_->keyParam_.at(nowScene_).sceneNumber;
	// UV座標の更新
	UpdateTrans(number);
	oldPath_ = path;
	return true;
}

void Animation2D::Play(bool flag) {
	if (isPlay_ == flag) { return; }
	isPlay_ = flag;
	nowFrame_ = 0.0f;
	nowScene_ = 0u;
}

Transform Animation2D::GetSceneUV(const uint32_t& scene) {
	UpdateTrans(scene);
	return transform_;
}

bool Animation2D::AnimationCount() {
	if (data_->keyParam_.at(nowScene_).keyFrame <= nowFrame_++) {
		nowScene_ += 1u;
		nowFrame_ = 0.0f;
		// 最後まで行った&&ループするならば
		if (nowScene_ == data_->keyParam_.size() && isLoop_) {
			nowScene_ = 0u;
		}
		return true;
	}
	return false;
}

void Animation2D::UpdateTrans(const uint32_t& listNum) {
	transform_.scale_ = data_->texParam_.uvScale;
	transform_.rotate_ = Vector3(0.0f, 0.0f, 0.0f);
	transform_.translate_ = data_->sceneNumberList_.at(listNum);
	transform_.UpdateMatrix();
}