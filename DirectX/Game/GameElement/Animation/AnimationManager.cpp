#include "AnimationManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include "GlobalVariables/GlobalVariables.h"
#include "TextureManager.h"


AnimationManager::AnimationManager() {
	model_ = std::make_unique<Model>("plane");
	model_->transform_.scale_ = Vector3(5.0f, 5.0f, 1.0f);
	sprite_ = std::make_unique<Sprite>();
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	Initialize();
}

AnimationManager::~AnimationManager() {
	if (animation_) {
		animation_.release();
	}
}

AnimationManager* AnimationManager::GetInstance() {
	static AnimationManager instance;
	return &instance;
}

void AnimationManager::Update() {
#ifdef _DEBUG
	ImGuiProcess();
#endif // _DEBUG
	if (isEditor_) {
		if (animation_) {
			animation_->Update();
		}
		model_->Update();
	}
}

void AnimationManager::Draw(const Camera* camera) {
	if (isEditor_) {
		camera;
		model_->Draw(*camera_);
		sprite_->Draw();
	}
}

Animation2D* AnimationManager::AddAnimation(const std::string& groupName) {
	// 追加されていなければ追加する
	if (container_.find(groupName) == container_.end()) {
		container_.emplace(std::make_pair(groupName, std::make_unique<Animation2D>()));
		container_.at(groupName).get()->Initialize(groupName);
	}
	return container_.at(groupName).get();
}

void AnimationManager::Initialize() {
	//SetGlobalVariable();
}

void AnimationManager::ImGuiProcess() {
#ifdef _DEBUG
	ImGui::Begin("Animation作成", nullptr, ImGuiWindowFlags_MenuBar);
	if (ImGui::Button("Editor")) {
		isEditor_ = !isEditor_;
	}
	if (isEditor_) {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Initialize")) {
				ImGui::InputText("FileName", nameHandle_, sizeof(nameHandle_));

				if (ImGui::TreeNode("Texture")) {
					ImGui::InputText("TextureName", textureName_, sizeof(textureName_));
					if (ImGui::Button("ChangeTexture")) {
						// 板ポリにtextureのセット
						auto texHandle = TextureManager::GetInstance()->LoadTexture(textureName_);
						model_->SetTexture(texHandle);
						sprite_->SetTexture(texHandle);
					}
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Create")) {
					int handle = static_cast<int>(divisionHeight);
					ImGui::DragInt("縦分割数", &handle, 1, 1, 50);
					divisionHeight = static_cast<uint32_t>(handle);
					handle = static_cast<int>(divisionWidth);
					ImGui::DragInt("横分割数", &handle, 1, 1, 50);
					divisionWidth = static_cast<uint32_t>(handle);

					// アニメーション生成
					if (ImGui::Button("Create")) {
						if (animation_) {
							animation_.reset(nullptr);
						}
						animation_.release();
						animation_.reset(AddAnimation(nameHandle_));
						animation_->Initialize(nameHandle_, divisionWidth, divisionHeight);
					}
					ImGui::TreePop();
				}

				if (ImGui::Button("Play")) {
					animation_->Play();
				}
				if (ImGui::Button("Load")) {
					animation_.release();
					animation_.reset(AddAnimation(nameHandle_));
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}
	ImGui::End();
#endif // _DEBUG
}
