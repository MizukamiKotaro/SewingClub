#include "TitleScene.h"
#include "Kyoko.h"
#include "ImGuiManager/ImGuiManager.h"
#include"Audio/AudioManager/AudioManager.h"

TitleScene::TitleScene()
{
	FirstInit();

	buttonA_ = std::make_unique<Sprite>("space.png");
	buttonA_->pos_.y = 500.0f;
	
	titleLogo_ = std::make_unique<Sprite>("titleLogo_1.png");


	bgm_.LoadWave("Music/title.wav","TitleBGM",bgmVolume_);
}

void TitleScene::Initialize()
{
	AudioManager::GetInstance()->AllStop();

	bgm_.Play(true);
}

void TitleScene::Update()
{
	buttonA_->Update();
	titleLogo_->Update();

	SceneChange();
}

void TitleScene::Draw()
{
	WrightPostEffect();

	Kyoko::Engine::PreDraw();

	buttonA_->Draw();
	titleLogo_->Draw();

	BlackDraw();

	Kyoko::Engine::PostDraw();
}

void TitleScene::WrightPostEffect()
{
	
}

void TitleScene::SceneChange()
{
	//
	if (input_->PressedGamePadButton(Input::GamePadButton::A)) {
		// シーン切り替え
		stageNo_ = 0;
		ChangeScene(SELECT);
		bgm_.Stop();
	}
}

