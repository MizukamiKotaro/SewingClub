#include "ClearScene.h"
#include "Kyoko.h"

ClearScene::ClearScene()
{
	FirstInit();

	dome_ = std::make_unique<Dome>();
	stage_ = std::make_unique<Stage>();
	screen_ = std::make_unique<Screen>();

	ui_ = std::make_unique<ClearUI>();

	space_ = std::make_unique<Sprite>("space.png");
	space_->pos_ = { 640.0f,560.0f };
	space_->Update();
	isEnd_ = false;

	se_.LoadWave("SE/select.wav");
}

void ClearScene::Initialize()
{
	ui_->Initialize();
	isEnd_ = false;
}

void ClearScene::Update()
{
	isEnd_ = ui_->IsEnd();

	ui_->Update();

	if ((input_->PressedKey(DIK_RETURN) || input_->PressedGamePadButton(Input::GamePadButton::A)) && isEnd_) {
		// シーン切り替え
		ChangeScene(SELECT);
		se_.Play();
	}
}

void ClearScene::Draw()
{
	screen_->PreDrawScene();
	ui_->Draw();
	screen_->PostDrawScene();

	Kyoko::Engine::PreDraw();

	dome_->Draw(camera_.get());
	stage_->Draw(camera_.get());
	screen_->Draw(camera_.get());

	if (isEnd_) {
		space_->Draw();
	}

	BlackDraw();

	Kyoko::Engine::PostDraw();
}


