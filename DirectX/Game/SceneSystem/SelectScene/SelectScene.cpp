#include "SelectScene.h"
#include "Kyoko.h"

SelectScene::SelectScene()
{
	FirstInit();

	dome_ = std::make_unique<Dome>();
	stage_ = std::make_unique<Stage>();
	screen_ = std::make_unique<Screen>();

	musicSprites_[SHINING_STAR] = std::make_unique<Sprite>("shining_star.png");
	musicSprites_[SHINING_STAR]->pos_ = { 640.0f,360.0f };
	musicSprites_[SHINING_STAR]->size_ *= 1.5f;
	musicSprites_[SHINING_STAR]->Update();

	space_ = std::make_unique<Sprite>("space.png");
	space_->pos_ = { 640.0f,560.0f };
	space_->Update();

	se_.LoadWave("SE/select.wav");
}

void SelectScene::Initialize()
{

}

void SelectScene::Update()
{
	if (input_->PressedGamePadButton(Input::GamePadButton::A)) {
		// シーン切り替え
		stageNo_ = 0;
		ChangeScene(STAGE);
		se_.Play();
	}
}

void SelectScene::Draw()
{
	screen_->PreDrawScene();
	musicSprites_[IScene::stageNo_]->Draw();
	screen_->PostDrawScene();

	Kyoko::Engine::PreDraw();

	dome_->Draw(camera_.get());
	stage_->Draw(camera_.get());
	screen_->Draw(camera_.get());

	space_->Draw();

	BlackDraw();

	Kyoko::Engine::PostDraw();
}

