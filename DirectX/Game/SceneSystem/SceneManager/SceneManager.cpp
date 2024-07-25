#include "SceneManager.h"

#include "Kyoko.h"
#include "Input.h"
#include "FrameInfo/FrameInfo.h"
#include "ImGuiManager/ImGuiManager.h"
#include "GameElement/ScoreManager/ScoreManager.h"
#include "SceneSystem/SceneFactory/SceneFactory.h"
#include"GameElement//SceneAcquisition/SceneAcquisition.h"

#ifdef _DEBUG
Vector4 color = { 1.0f,1.0f,1.0f,1.0 };
#endif // _DEBUG


SceneManager::SceneManager()
{
	ScoreManager::GetInstance()->Initialize();

	IScene::sceneNo_ = TITLE;
	
	sceneName_.clear();
	sceneName_.push_back("TITLE");
	sceneName_.push_back("SELECT");
	sceneName_.push_back("STAGE");
	sceneName_.push_back("CLEAAR");
	sceneName_.push_back("GAMEOVER");


	sceneFactory_ = std::make_unique<SceneFactory>();

	scene_.reset(sceneFactory_->CreateScene(IScene::sceneNo_));

	currentSceneNo_ = IScene::sceneNo_;
	preSceneNo_ = currentSceneNo_;
	IScene::stageNo_ = 0;

	inputManager_ = Input::GetInstance();
	frameInfo_ = FrameInfo::GetInstance();

	
}

int SceneManager::Run()
{
	scene_->Initialize();

	SceneAcquisition::GetInstance()->Initialize();

	// ウィンドウの×ボタンが押されるまでループ
	while (true) {
		// フレームの開始
		if (Kyoko::Engine::ProcessMessage() || (inputManager_->PressedKey(DIK_ESCAPE) && IScene::sceneNo_ == TITLE)||IScene::isBreak_) {
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

		DebugWindow();
		
		scene_->Draw();

		frameInfo_->End();
	}

	SceneAcquisition::GetInstance()->Finalize();
	scene_.reset();
	sceneFactory_.reset();

	return 0;
}

void SceneManager::DebugWindow()
{

//#ifdef _DEBUG
//	ImGui::Begin("SCENE");
//	switch (currentSceneNo_)
//	{
//	case SCENE::SELECT:
//		ImGui::Text("SELECT");
//		break;
//	case SCENE::TITLE:
//		ImGui::Text("TITLE");
//		break;
//	case SCENE::STAGE:
//		ImGui::Text("STAGE");
//		break;
//	case SCENE::CLEAR:
//		ImGui::Text("CLEAR");
//		break;
//	default:
//		break;
//	}
//	ImGui::End();
//
//	ImGui::Begin("フレームレート");
//	ImGui::Text("フレーム : %4.1f", frameInfo_->GetFramerate());
//	ImGui::End();
//#endif // _DEBUG

#ifdef _DEBUG

	int num = IScene::sceneNo_;

	ImGui::Begin("SceneManager");
	ImGui::Text("SceneNo.%d", currentSceneNo_);
	ImGui::Text("%s", sceneName_[currentSceneNo_].c_str());
	ImGui::SliderInt("sceneNo", &num, 0, _SceneCount - 1);
	ImGui::Text("フレーム : %4.1f", frameInfo_->GetFramerate());
	ImGui::End();
	ImGui::Begin("色の確認");
	ImGui::ColorEdit4("カラーの数値確認", &color.x);
	ImGui::Text("R : %f G : %f B : %f", color.x, color.y, color.z);
	ImGui::End();
	IScene::sceneNo_ = num;
#endif // _DEBUG

}