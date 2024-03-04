#include "SceneManager.h"

#include "Kyoko.h"
#include "Input.h"
#include "FrameInfo/FrameInfo.h"
#include "ImGuiManager/ImGuiManager.h"
#include "GameElement/ScoreManager/ScoreManager.h"
#include "SceneSystem/SceneFactory/SceneFactory.h"

SceneManager::SceneManager()
{
	ScoreManager::GetInstance()->Initialize();

	IScene::sceneNo_ = TITLE;

	sceneFactory_ = std::make_unique<SceneFactory>();

	scene_.reset(sceneFactory_->CreateScene(IScene::sceneNo_));

	currentSceneNo_ = IScene::sceneNo_;
	preSceneNo_ = currentSceneNo_;
	IScene::stageNo_ = 0;

	inputManager_ = Input::GetInstance();
}

int SceneManager::Run()
{
	scene_->Initialize();

	// ウィンドウの×ボタンが押されるまでループ
	while (true) {
		// フレームの開始
		if (Kyoko::Engine::ProcessMessage() || (inputManager_->PressedKey(DIK_ESCAPE) && IScene::sceneNo_ == TITLE)) {
			break;
		}

		Kyoko::Engine::FirstUpdateInLoop();

		// 更新処理
		preSceneNo_ = currentSceneNo_;
		currentSceneNo_ = IScene::sceneNo_;

		if (preSceneNo_ != currentSceneNo_) {
			scene_.reset(sceneFactory_->CreateScene(IScene::sceneNo_));
			scene_->Initialize();
		}

		scene_->Play();

#ifdef _DEBUG
		ImGui::Begin("SCENE");
		switch (currentSceneNo_)
		{
		case SCENE::SELECT:
			ImGui::Text("SELECT");
			break;
		case SCENE::TITLE:
			ImGui::Text("TITLE");
			break;
		case SCENE::STAGE:
			ImGui::Text("STAGE");
			break;
		case SCENE::CLEAR:
			ImGui::Text("CLEAR");
			break;
		default:
			break;
		}
		ImGui::End();
#endif // _DEBUG
		
		scene_->Draw();

		FrameInfo::GetInstance()->End();
	}

	scene_.reset();
	sceneFactory_.reset();

	return 0;
}