#include "SelectScene.h"
#include "Kyoko.h"

SelectScene::SelectScene()
{
	FirstInit();

	
}

void SelectScene::Initialize()
{

}

void SelectScene::Update()
{
	SceneChange();
}

void SelectScene::Draw()
{
	
	//必須
	Kyoko::Engine::PreDraw();

	
	BlackDraw();

	//必須
	Kyoko::Engine::PostDraw();
}

void SelectScene::SceneChange()
{
	if (input_->PressedGamePadButton(Input::GamePadButton::A)) {
		// シーン切り替え
		stageNo_ = 0;
		ChangeScene(STAGE);
	}
}

