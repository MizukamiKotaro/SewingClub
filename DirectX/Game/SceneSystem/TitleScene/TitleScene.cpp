#include "TitleScene.h"
#include "Kyoko.h"
#include "ImGuiManager/ImGuiManager.h"

TitleScene::TitleScene()
{
	FirstInit();


}

void TitleScene::Initialize()
{

}

void TitleScene::Update()
{
	SceneChange();
}

void TitleScene::Draw()
{
	WrightPostEffect();

	Kyoko::Engine::PreDraw();

	

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
	}
}

