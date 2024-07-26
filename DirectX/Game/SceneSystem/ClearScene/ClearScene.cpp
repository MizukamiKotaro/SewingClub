#include "ClearScene.h"
#include "Kyoko.h"
#include"Ease/Ease.h"

ClearScene::ClearScene()
{
	FirstInit();

	sceneAcuition_ = SceneAcquisition::GetInstance();

	gameClear_ = std::make_unique<GameClear>();

	sceneTransition_ = std::make_unique<SceneTransitionEffect>("clear");

	if (stageNo_ + 1 == maxStageNo_) {
		gameClear_->Initialize(stageNo_, false);
	}
	else {
		gameClear_->Initialize(stageNo_, true);
	}
	gameClear_->SetBabyParam();
	sceneTransition_->Initialize(1.0f);
	isDossolve_ = false;
}

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{

}

void ClearScene::Update()
{
	float deltaTime = frameInfo_->GetDeltaTime();
	gameClearFlags_ = gameClear_->Update(deltaTime);

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

void ClearScene::FromBlackInitialize()
{
	if (sameScene_) {
		sameScene_ = false;
		Initialize();
	}

	transitionTimeCount_ = 10.0f;
	black_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
}

void ClearScene::FromBlackUpdate()
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



void ClearScene::ToBlackInitialize()
{
	black_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
	sceneTransition_->Initialize(1.0f);
}

void ClearScene::ToBlackUpdate()
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

void ClearScene::SceneChange()
{


	if (gameClearFlags_.goNeext) {
		// シーン切り替え
		if (stageNo_ + 1 == maxStageNo_) {
			ChangeScene(SELECT);
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
		ChangeScene(SELECT);
		isDossolve_ = true;
	}

}
