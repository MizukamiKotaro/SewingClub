#include "StageScene.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Kyoko.h"

StageScene::StageScene()
{
	FirstInit();

	game_ = std::make_unique<Game>(camera_.get());
}

void StageScene::Initialize()
{
	game_->Initialize();
}

void StageScene::Update()
{
	if (input_->PressedKey(DIK_RETURN) || game_->GetIsClear()) {
		// シーン切り替え
		ChangeScene(CLEAR);
	}

#ifdef _DEBUG
	

#endif // _DEBUG

	game_->Update();
}

void StageScene::Draw()
{


	Kyoko::Engine::PreDraw();

	game_->Draw();

	BlackDraw();

	// フレームの終了
	Kyoko::Engine::PostDraw();
}

