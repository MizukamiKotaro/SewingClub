#include "GameOverScene.h"
#include "Kyoko.h"


GameOverScene::GameOverScene()
{
	FirstInit();

	sceneAcuition_ = SceneAcquisition::GetInstance();
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
}

void GameOverScene::Update()
{
}

void GameOverScene::Draw()
{
	Kyoko::Engine::PreDraw();

	sceneAcuition_->DrawScene();

	Kyoko::Engine::PostDraw();
}
