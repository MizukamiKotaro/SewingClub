#include "OptionScene.h"
#include "Kyoko.h"

OptionScene::OptionScene()
{
	FirstInit();

	sceneAcuition_ = SceneAcquisition::GetInstance();

	optionUI_ = std::make_unique<OptionUI>(sceneAcuition_->PreSceneName_);

}

OptionScene::~OptionScene()
{
}

void OptionScene::Initialize()
{
	optionUI_->Initialize();
	optionUI_->Update();


}

void OptionScene::Update()
{
	UpdateAnswer ans = optionUI_->Update();

	
}

void OptionScene::Draw()
{
	Kyoko::Engine::PreDraw();

	sceneAcuition_->DrawScene();

	optionUI_->Draw();

	Kyoko::Engine::PostDraw();

}
