#include "PopupUI.h"
#include "Utils/Ease/Ease.h"
#include <algorithm>

PopupUI::PopupUI() {
	sprite_ = std::make_unique<Sprite>();
	sprite_->LoadTexture("ingame_start_HUD.png");
}

void PopupUI::Initialize() {
	phase = Phase::PopUp;
	counter_.Initialize();
	isFinished_ = false;
	alpha_ = 1.0f;
	scale_ = Vector2(0.0f, 0.0f);
	sprite_->pos_.y = 500.0f;
	SpriteUpdate();
}

void PopupUI::Update(const float& delta) {
	
	if (isFinished_) { return; }
	switch (phase) {
	case PopupUI::Phase::PopUp:
		PopUp();
		break;
	case PopupUI::Phase::Wait:
		Wait();
		break;
	case PopupUI::Phase::FadeOut:
		FadeOut();
		break;
	}

	counter_.nowFrame += delta;
	SpriteUpdate();
}

void PopupUI::Draw() {
	if (isFinished_) { return; }
	sprite_->Draw();
}

void PopupUI::SpriteUpdate() {
	sprite_->SetColor(Vector4(1.0f, 1.0f, 1.0f, alpha_));
	sprite_->size_ = scale_;
	sprite_->Update();
}

void PopupUI::PopUp() {
	counter_.nowFrame = std::clamp(counter_.nowFrame, 0.0f, counter_.kMaxPopFrame);
	scale_ = Ease::UseEase(Vector2(0.0f,0.0f), kMaxSize_, counter_.nowFrame, counter_.kMaxPopFrame, Ease::EaseType::EaseOut, 4);
	if (counter_.nowFrame >= counter_.kMaxPopFrame) {
		phase = Phase::Wait;
		counter_.Initialize();
	}
}

void PopupUI::Wait() {
	counter_.nowFrame = std::clamp(counter_.nowFrame, 0.0f, counter_.kMaxWaitFrame);
	if (counter_.nowFrame >= counter_.kMaxWaitFrame) {
		phase = Phase::FadeOut;
		counter_.Initialize();
	}
}

void PopupUI::FadeOut() {
	counter_.nowFrame = std::clamp(counter_.nowFrame, 0.0f, counter_.kMaxFadeFrame);
	alpha_ = Ease::UseEase(1.0f, 0.0f, counter_.nowFrame, counter_.kMaxFadeFrame, Ease::EaseType::EaseOut, 3);
	if (counter_.nowFrame >= counter_.kMaxFadeFrame) {
		phase = Phase::PopUp;
		counter_.Initialize();
		isFinished_ = true;
	}
}
