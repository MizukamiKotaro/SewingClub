#include "GameOverScene.h"
#include "Kyoko.h"


GameOverScene::GameOverScene()
{
	FirstInit();

	sceneAcuition_ = SceneAcquisition::GetInstance();

	gameOver_ = std::make_unique<GameOver>();

	sceneTransition_ = std::make_unique<SceneTransitionEffect>("gameOver");
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
	//初期化
	gameOver_->Initialize();
	sceneTransition_->Initialize(1.0f);
}

void GameOverScene::Update()
{
	//ゲームオーバー更新処理
	gameOverFlags_ = gameOver_->Update();

	//シーン変更処理
	SceneChange();
}

void GameOverScene::Draw()
{
	//Dissolve描画前処理
	sceneTransition_->DrawPE();

	//エンジン描画前処理
	Kyoko::Engine::PreDraw();

	//前シーン描画
	sceneAcuition_->DrawScene();

	//ゲームクリアのUI描画
	gameOver_->Draw();

	//遷移描画
	BlackDraw();

	//Dissolve描画
	sceneTransition_->Draw();

	//エンジン描画後処理
	Kyoko::Engine::PostDraw();
}

void GameOverScene::SceneChange()
{
	//遷移時
	if (isChangeScene_) {
		float deltaTime = frameInfo_->GetDeltaTime();
		if (sceneTransition_->PostSceneTransition(deltaTime)) {
			ChangeScene(SELECT);
		}
	}//遷移前処理
	else {
		//リスタート処理
		if (gameOverFlags_.restart) {
			ChangeScene(STAGE);
		}
		//セレクト処理
		if (gameOverFlags_.goSelect) {
			isChangeScene_ = true;

		}
	}
}
