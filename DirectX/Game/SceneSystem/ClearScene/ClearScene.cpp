#include "ClearScene.h"
#include "Kyoko.h"


ClearScene::ClearScene()
{
	FirstInit();

	sceneAcuition_ = SceneAcquisition::GetInstance();
}

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{
}

void ClearScene::Update()
{
}

void ClearScene::Draw()
{
	Kyoko::Engine::PreDraw();

	sceneAcuition_->DrawScene();

	Kyoko::Engine::PostDraw();
}
