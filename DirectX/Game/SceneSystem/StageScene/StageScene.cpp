#include "StageScene.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Kyoko.h"

StageScene::StageScene()
{
	FirstInit();

	Yarn::StaticInitialize();

	instancingmodelManager_ = InstancingModelManager::GetInstance();

	player_ = std::make_unique<Player>();
}

void StageScene::Initialize()
{
	player_->Initialize();
}

void StageScene::Update()
{
	if (input_->PressedKey(DIK_RETURN)) {
		// シーン切り替え
		ChangeScene(CLEAR);
	}

#ifdef _DEBUG
	Yarn::StaticUpdate();

#endif // _DEBUG

	player_->Update();
}

void StageScene::Draw()
{
	instancingmodelManager_->Clear();

	Kyoko::Engine::PreDraw();

	player_->Draw(camera_.get());

	instancingmodelManager_->Draw(*camera_.get());

	BlackDraw();

	// フレームの終了
	Kyoko::Engine::PostDraw();
}

