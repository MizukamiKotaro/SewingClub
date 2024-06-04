#include "TensionUI.h"
#include "ImGuiManager/ImGuiManager.h"
#include "TextureManager/TextureManager.h"
#include "calc.h"

TensionUI::TensionUI() {
	for (auto& sprite : sprites_) {
		sprite = std::make_unique<Sprite>(TextureManager::GetInstance()->LoadTexture("baby_UI_gageDown.png"));
	}
	sprites_.at(static_cast<uint32_t>(Type::Face))->LoadTexture("baby_face.png");
	global_ = std::make_unique<GlobalVariableUser>("UI", "TensionUI");
	tree = {
	   "Frame",
	   "Gauge",
	   "Face",
	};
	animationData_ = std::make_unique<Animation2DData>();
	animationData_->Initialize(1u, 9u);
	animation_ = std::make_unique<Animation2D>(animationData_.get());
}

void TensionUI::Initialize(const float& tension, const int& faceParam) {
	SetGlobalVariable();
	sprites_.at(static_cast<uint32_t>(Type::Frame))->SetAnchorPoint(Vector2(0.5f, 0.56f));
	sprites_.at(static_cast<uint32_t>(Type::Frame))->SetColor(Vector4(0.21f, 0.20f, 0.19f, 1.0f));

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
	float spriteSize = kMaxSize_.x * uvScale;
	// spriteの位置を求める (最大サイズ - 今のサイズ) / 2 + 定位置
	float spritePos = (kMaxSize_.x - spriteSize) * 0.5f;
	spritePos += fixedPosition_.x;

	// 座標更新
	sprites_.at(static_cast<uint32_t>(Type::Gauge))->SetTextureTopLeft(Vector2(uvTrans, 0.0f));
	sprites_.at(static_cast<uint32_t>(Type::Gauge))->SetTextureSize(Vector2(uvScale, 1.0f));
	sprites_.at(static_cast<uint32_t>(Type::Gauge))->pos_ = Vector2(spritePos, fixedPosition_.y);
	sprites_.at(static_cast<uint32_t>(Type::Gauge))->size_ = Vector2(spriteSize, kMaxSize_.y);

	// 表情の更新
	auto handle = animation_->GetSceneUV(faceParam);
	sprites_.at(static_cast<uint32_t>(Type::Face))->SetTextureTopLeft(Vector2(handle.translate_.x, handle.translate_.y));
	sprites_.at(static_cast<uint32_t>(Type::Face))->SetTextureSize(Vector2(handle.scale_.x, handle.scale_.y));

	GaugeColor(tension);

	for (auto& sprite : sprites_) {
		sprite->Update();
	}
}

void TensionUI::Update(const float& tension, const int& faceParam) {
#ifdef _DEBUG
	ApplyGlobalVariable();
	ImGui::Begin("テンション");
	static float anchor = 0.1f;
	ImGui::DragFloat("テンション率", &tensionPercent_, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("アンカーポイント", &anchor, 0.01f, 0.0f, 1.0f);
	ImGui::Text("Face %d", faceParam);
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
	float spriteSize = kMaxSize_.x * uvScale;
	// spriteの位置を求める (最大サイズ - 今のサイズ) / 2 + 定位置
	float spritePos = (kMaxSize_.x - spriteSize) * 0.5f;
	spritePos += fixedPosition_.x;

	spritePos = Calc::Lerp(sprites_.at(static_cast<uint32_t>(Type::Gauge))->pos_.x, spritePos, 0.1f);
	spriteSize = Calc::Lerp(sprites_.at(static_cast<uint32_t>(Type::Gauge))->size_.x, spriteSize, 0.1f);

	// 座標更新
	sprites_.at(static_cast<uint32_t>(Type::Gauge))->SetTextureTopLeft(Vector2(uvTrans,0.0f));
	sprites_.at(static_cast<uint32_t>(Type::Gauge))->SetTextureSize(Vector2(uvScale, 1.0f));
	sprites_.at(static_cast<uint32_t>(Type::Gauge))->pos_ = Vector2(spritePos, fixedPosition_.y);
	sprites_.at(static_cast<uint32_t>(Type::Gauge))->size_ = Vector2(spriteSize, kMaxSize_.y);

	// 表情の更新
	auto handle = animation_->GetSceneUV(faceParam);
	sprites_.at(static_cast<uint32_t>(Type::Face))->SetTextureTopLeft(Vector2(handle.translate_.x, handle.translate_.y));
	sprites_.at(static_cast<uint32_t>(Type::Face))->SetTextureSize(Vector2(handle.scale_.x, handle.scale_.y));
	
	GaugeColor(tension);

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
	global_->AddItem("定位置", fixedPosition_);
	global_->AddItem("最大サイズ", kMaxSize_);

	uint32_t index = 0u;
	for (auto& sprite : sprites_) {
		if (index != static_cast<uint32_t>(Type::Gauge)) {
			global_->AddItem("size", sprite->size_, tree.at(index));
			global_->AddItem("pos", sprite->pos_, tree.at(index));
		}
		global_->AddItem("color", Vector3(colors_.at(index).x, colors_.at(index).y, colors_.at(index).z), tree.at(index));
		global_->AddItem("透明度a", colors_.at(index).w, tree.at(index));
		index++;
	}
	ApplyGlobalVariable();
}

void TensionUI::ApplyGlobalVariable() {
	fixedPosition_ = global_->GetVector2Value("定位置");
	kMaxSize_ = global_->GetVector2Value("最大サイズ");

	uint32_t index = 0u;
	for (auto& sprite : sprites_) {
		if (index != static_cast<uint32_t>(Type::Gauge)) {
			sprite->size_ = global_->GetVector2Value("size", tree.at(index));
			sprite->pos_ = global_->GetVector2Value("pos", tree.at(index));
		}
		Vector3 colorHandle = global_->GetVector3Value("color", tree.at(index));
		float alphaHandle = global_->GetFloatValue("透明度a", tree.at(index));
		colors_.at(index) = Vector4(colorHandle.x, colorHandle.y, colorHandle.z, alphaHandle);
		sprite->SetColor(colors_.at(index));
		// フレームは全部親子関係結ぶ
		if (index == static_cast<uint32_t>(Type::Frame)) {
			sprite->size_ += kMaxSize_;
			sprite->pos_ += fixedPosition_;
		}
		// 表情は座標だけ
		else if (index == static_cast<uint32_t>(Type::Face)) {
			sprite->pos_ += fixedPosition_;
		}
		index++;
	}
}

void TensionUI::GaugeColor(const float& tension) {
	Vector4 color{};
	if (tension >= 90.0f) {
		color = Vector4(0.20f, 0.89f, 0.20f, 1.0f);
	}
	else if (tension <= 30.0f) {
		color = Vector4(0.88f, 0.38f, 0.20f, 1.0f);
	}
	else {
		color = Vector4(0.89f, 0.76f, 0.20f, 1.0f);
	}
	sprites_.at(static_cast<uint32_t>(Type::Gauge))->SetColor(color);
}
