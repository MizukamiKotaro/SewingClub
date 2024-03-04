#include "TitleScene.h"
#include "Kyoko.h"
#include "ImGuiManager/ImGuiManager.h"

TitleScene::TitleScene()
{
	FirstInit();

	player_ = std::make_unique<Player>();
}

void TitleScene::Initialize()
{

}

void TitleScene::Update()
{
	player_->Update();
}

void TitleScene::Draw()
{
	WrightPostEffect();

	Kyoko::Engine::PreDraw();

	player_->Draw(camera_.get());

	BlackDraw();

	Kyoko::Engine::PostDraw();
}

void TitleScene::WrightPostEffect()
{
	
}

