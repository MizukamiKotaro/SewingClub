#include "ClearScene.h"
#include "Kyoko.h"


ClearScene::ClearScene()
{
	FirstInit();

	sceneAcuition_ = SceneAcquisition::GetInstance();

	gameClear_ = std::make_unique<GameClear>();

	sceneTransition_ = std::make_unique<SceneTransitionEffect>("clear");
}

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{
	if (stageNo_ + 1 == maxStageNo_) {
		gameClear_->Initialize(stageNo_, false);
	}
	else {
		gameClear_->Initialize(stageNo_, true);
	}

	sceneTransition_->Initialize(1.0f);
}

void ClearScene::Update()
{
	float deltaTime = frameInfo_->GetDeltaTime();
	gameClearFlags_=gameClear_->Update(deltaTime);

	SceneChange();
}

void ClearScene::Draw()
{

	sceneTransition_->DrawPE();
	
	Kyoko::Engine::PreDraw();

	sceneAcuition_->DrawScene();

	gameClear_->Draw();

	BlackDraw();

	sceneTransition_->Draw();

	
	Kyoko::Engine::PostDraw();
}

void ClearScene::SceneChange()
{

	//遷移処理フラグ
	if (isChangeScene_) {
		float deltaTime = frameInfo_->GetDeltaTime();
		if (sceneTransition_->PostSceneTransition(deltaTime)) {
			ChangeScene(SELECT);
		}
	}
	else {
		if (gameClearFlags_.goNeext) {
			// シーン切り替え
			if (stageNo_ + 1 == maxStageNo_) {
				isChangeScene_ = true;
			}
			else {
				stageNo_++;
				ChangeScene(STAGE);
			}


		}
		else if (gameClearFlags_.reTry) {
			ChangeScene(STAGE);

		}
		else if (gameClearFlags_.goSelect) {
			isChangeScene_ = true;		
		}
	}
}
