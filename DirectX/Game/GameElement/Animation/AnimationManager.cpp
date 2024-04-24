#include "AnimationManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include "GlobalVariables/GlobalVariables.h"
#include "TextureManager.h"


AnimationManager::AnimationManager() {
	model_ = std::make_unique<Model>("plane");
	camera_ = std::make_unique<Camera>();
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
	if (isEditor_) {
		if (animation_) {
			//animation_->Update();
			//model_->SetUVParam(animation_->GetUVTrans());
		}
		model_->Update();
		spritesheet_->Update();
	}
#endif // _DEBUG
}

void AnimationManager::Draw(const Camera* camera) {
	if (isEditor_) {
		camera;
		model_->Draw(*camera_);
		spritesheet_->Draw(*camera_);
	}
}

Animation2DData* AnimationManager::AddAnimation(const std::string& groupName) {
	// 追加されていなければ追加する
	if (container_.find(groupName) == container_.end()) {
		container_.emplace(std::make_pair(groupName, std::make_unique<Animation2DData>()));
		container_.at(groupName).get()->Initialize(groupName);
	}
	return container_.at(groupName).get();
}

void AnimationManager::Initialize() {
	spritesheet_ = std::make_unique<Model>("plane");
	model_->transform_.scale_ = Vector3(3.0f, 3.0f, 1.0f);
	model_->transform_.translate_ = Vector3(0.0f, 0.0f, 1.0f);
	model_->Update();
	spritesheet_->transform_.scale_ = Vector3(4.0f, 2.5f, 1.0f);
	spritesheet_->transform_.translate_ = Vector3(-6.0f, 4.5f, 1.0f);
	spritesheet_->Update();
	camera_->Initialize();
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
						spritesheet_->SetTexture(texHandle);
					}
					ImGui::DragFloat3("modelTrans", &model_->transform_.translate_.x, 0.1f);
					ImGui::DragFloat3("modelScale", &model_->transform_.scale_.x, 0.1f);
					ImGui::DragFloat3("spritemodelTrans", &spritesheet_->transform_.translate_.x, 0.1f);
					ImGui::DragFloat3("spritemodelScale", &spritesheet_->transform_.scale_.x, 0.1f);
					
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
					//animation_->Play(!animation_->isPlay_);
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
