#include "TensionUI.h"
#include "ImGuiManager/ImGuiManager.h"
#include "TextureManager/TextureManager.h"

TensionUI::TensionUI() {
	for (auto& model : sprites_) {
		model = std::make_unique<Sprite>(TextureManager::GetInstance()->LoadTexture("baby_UI_gageDown.png"));
	}
	global_ = std::make_unique<GlobalVariableUser>("UI", "TensionUI");
	tree = {
	   "Frame",
	   "Gauge",
	};
}

void TensionUI::Initialize() {
	SetGlobalVariable();
	//sprites_.at(static_cast<uint32_t>(Type::Gauge))->transform_.parent_ = &sprites_.at(static_cast<uint32_t>(Type::Frame))->transform_;
	sprites_.at(static_cast<uint32_t>(Type::Frame))->SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
}

void TensionUI::Update(const float& tension) {
#ifdef _DEBUG
	ApplyGlobalVariable();
	ImGui::Begin("テンション");
	ImGui::DragFloat("テンション率", &tensionPercent_, 0.01f, 0.0f, 1.0f);
	ImGui::End();
#endif // _DEBUG

	// ここで0 ~ 100 なのを 0 ~ 1でもらうようにしている
	tensionPercent_ = tension * 0.01f;

	/*
	* pos,size,uvScale,uvTrans
	* 初期値 1060.0f,350.0f,1.0f,1.0f
	* 1097.5f,280.0f,0.8f,0.2fだった場合
	*/

	// uvを求める xのみでyはかならず固定
	// uvScaleを求める テンション率
	float uvScale = tensionPercent_;
	// uv座標を求める 1.0f - テンション率
	float uvTrans = 1.0f - tensionPercent_;
	// spriteSizeを求める 最大サイズ * uvScale
	float spriteSize = kMaxSize_ * uvScale;
	// spriteの位置を求める (最大サイズ - 今のサイズ) / 2 + 定位置
	float spritePos = (kMaxSize_ - spriteSize) * 0.5f;
	spritePos += fixedPosition_;

	sprites_.at(static_cast<uint32_t>(Type::Gauge))->SetTextureTopLeft(Vector2(uvTrans,0.0f));
	sprites_.at(static_cast<uint32_t>(Type::Gauge))->SetTextureSize(Vector2(uvScale, 1.0f));
	sprites_.at(static_cast<uint32_t>(Type::Gauge))->pos_.x = spritePos;
	sprites_.at(static_cast<uint32_t>(Type::Gauge))->size_.x = spriteSize;
	for (auto& sprite : sprites_) {
		sprite->Update();
	}
}

void TensionUI::Draw() {
	for (auto& sprite : sprites_) {
		sprite->Draw();
	}
}

void TensionUI::SetGlobalVariable() {
	uint32_t index = 0u;
	for (auto& sprite : sprites_) {
		global_->AddItem("size", sprite->size_, tree.at(index));
		global_->AddItem("pos", sprite->pos_, tree.at(index));
		index++;
	}
	ApplyGlobalVariable();
}

void TensionUI::ApplyGlobalVariable() {
	uint32_t index = 0u;
	for (auto& sprite : sprites_) {
		sprite->size_ = global_->GetVector2Value("size", tree.at(index));
		sprite->pos_ = global_->GetVector2Value("pos", tree.at(index));
		index++;
	}
}
