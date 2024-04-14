#include "AnimationManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include "GlobalVariables/GlobalVariables.h"
#include "TextureManager.h"


AnimationManager::AnimationManager() {
	global_ptr = GlobalVariables::GetInstance();
	model_ = std::make_unique<Model>("plane");
	model_->transform_.scale_ = Vector3(5.0f, 5.0f, 1.0f);
	Initialize();
}

void AnimationManager::Update() {
#ifdef _DEBUG
	//ImGuiProcess();
#endif // _DEBUG

	if (animation_) {
		animation_->Update();
	}
	model_->Update();
}

void AnimationManager::Draw(const Camera* camera) {
	model_->Draw(*camera);
}

Animation2D* AnimationManager::FindAnimation(const std::string& groupName) {
	// 追加されていなければ追加する
	if (container_.find(groupName) == container_.end()) {
		container_.emplace(std::make_pair(groupName, std::make_unique<Animation2D>()));
	}
	return container_.at(groupName).get();
}

void AnimationManager::Initialize() {
	SetGlobalVariable();
}

void AnimationManager::ImGuiProcess() {
	ImGui::Begin("Animation", nullptr, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Initialize")) {
			ImGui::InputText("FileName", nameHandle_, sizeof(nameHandle_));
			if (ImGui::TreeNode("Create")) {
				int handle = static_cast<int>(divisionHeight);
				ImGui::DragInt("縦分割数", &handle, 1, 1, 100);
				handle = static_cast<int>(divisionWidth);
				ImGui::DragInt("横分割数", &handle, 1, 1, 100);

				// アニメーション生成
				if (ImGui::Button("Create")) {
					itemName_ = nameHandle_;
					// 板ポリにtextureのセット
					model_->SetTexture(TextureManager::GetInstance()->LoadTexture(itemName_));
					if (animation_) {
						animation_.reset(nullptr);
					}
					animation_ = std::make_unique<Animation2D>();
					animation_->Initialize(model_.get(), divisionHeight, divisionWidth);
				}
				ImGui::TreePop();
			}

			if (ImGui::Button("Save")) {
				SetGlobalVariable();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}

void AnimationManager::Editor() {

}

void AnimationManager::SetGlobalVariable() {
	global_ptr->CreateGroup(chunkName, nameHandle_);
	global_ptr->AddItem(chunkName, nameHandle_, "TextureName", itemName_);
	global_ptr->AddItem(chunkName, nameHandle_, "DivisionWidth", static_cast<int>(divisionWidth));
	global_ptr->AddItem(chunkName, nameHandle_, "DivisionHeight", static_cast<int>(divisionHeight));
}

void AnimationManager::ApplyGlobalVariable() {
	itemName_ = global_ptr->GetStringValue(chunkName, nameHandle_, "TextureName");
	// 板ポリにtextureのセット
	model_->SetTexture(TextureManager::GetInstance()->LoadTexture(itemName_));
	if (animation_) {
		animation_.reset(nullptr);
	}
	animation_ = std::make_unique<Animation2D>();
	animation_->Initialize(model_.get(), divisionHeight, divisionWidth);
}
