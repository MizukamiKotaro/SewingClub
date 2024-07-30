#include "GameOverScene.h"
#include "Kyoko.h"
#include"Ease/Ease.h"

GameOverScene::GameOverScene()
{
	if (!isIniialize_) {
		FirstInit();
		isIniialize_ = true;
	}
	black_->SetColor(Vector4{ 0.0f,0.0f,0.0f,0.0f });


	sceneAcuition_ = SceneAcquisition::GetInstance();

	gameOver_ = std::make_unique<GameOver>();

	sceneTransition_ = std::make_unique<SceneTransitionEffect>("gameover");
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
	black_->SetColor(Vector4{ 0.0f,0.0f,0.0f,0.0f });


	//初期化
	gameOver_->Initialize();
	sceneTransition_->Initialize(1.0f);
	isDossolve_ =false;


}

void GameOverScene::Update()
{

#ifdef _DEBUG
	sceneTransition_->Debug();
#endif // _DEBUG


	//ゲームオーバー更新処理
	gameOverFlags_ = gameOver_->Update();

	black_->Update();


	//シーン変更処理
	SceneChange();
}

void GameOverScene::Draw()
{
	//Dissolve描画前処理
	//sceneTransition_->DrawPE();

	//エンジン描画前処理
	Kyoko::Engine::PreDraw();

	//前シーン描画
	sceneAcuition_->DrawScene();

	//ゲームクリアのUI描画
	gameOver_->Draw();

	//遷移描画
	if (transition_ == Transition::kToBlack) {
		BlackDraw();
	}
	//Dissolve描画
	sceneTransition_->Draw();

	//エンジン描画後処理
	Kyoko::Engine::PostDraw();
}

void GameOverScene::FromBlackInitialize()
{
	if (sameScene_) {
		sameScene_ = false;
		Initialize();
	}

	transitionTimeCount_ = 10.0f;
	black_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
}

void GameOverScene::FromBlackUpdate()
{
	transitionTimeCount_ = 10.0f;
	float delta = FrameInfo::GetInstance()->GetDeltaTime();
	if (delta >= 0.3f) {
		return;
	}
	transitionTimeCount_ += delta;

	float alpha =
		Ease::UseEase(1.0f, 0.0f, transitionTimeCount_, kTransitionTime, Ease::EaseInSine, 2);
	black_->SetColor({ 0.0f, 0.0f, 0.0f, alpha });

	if (transitionTimeCount_ >= kTransitionTime) {
		black_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
		transitionRequest_ = Transition::kOperation;
	}
}

void GameOverScene::ToBlackInitialize()
{
	transitionTimeCount_ = 0;
	black_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
	sceneTransition_->Initialize(1.0f);
}

void GameOverScene::ToBlackUpdate()
{
	if (isDossolve_) {
		float deltaTime = frameInfo_->GetDeltaTime();
		if (sceneTransition_->PostSceneTransition(deltaTime)) {
			transitionRequest_ = Transition::kFromBlack;

			if (sceneNo_ != nextScene_) {
				sceneNo_ = nextScene_;
			}

		}
	}
	else {

		transitionTimeCount_ += FrameInfo::GetInstance()->GetDeltaTime();

		float alpha =
			Ease::UseEase(0.0f, 1.0f, transitionTimeCount_, kTransitionTime, Ease::EaseInSine, 2);
		black_->SetColor({ 0.0f, 0.0f, 0.0f, alpha });

		if (transitionTimeCount_ >= kTransitionTime) {
			transitionRequest_ = Transition::kFromBlack;
			black_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });

			if (sceneNo_ == nextScene_) {
				sameScene_ = true;
			}
			else {
				sceneNo_ = nextScene_;
			}
		}
	}
}

void GameOverScene::SceneChange()
{

	//リスタート処理
	if (gameOverFlags_.restart) {
		ChangeScene(STAGE);
	}
	//セレクト処理
	if (gameOverFlags_.goSelect) {
		ChangeScene(SELECT);
		isDossolve_ = true;
	}

}
