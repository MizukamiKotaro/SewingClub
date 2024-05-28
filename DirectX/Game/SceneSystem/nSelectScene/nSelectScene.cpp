#include "nSelectScene.h"

nSelectScene::nSelectScene()
{
	FirstInit();	

	optionUI_ = std::make_unique<OptionUI>(OptionUI::kSelect);

	bg_ = std::make_unique<BackGround>();
	bg_->Update(camera_.get());

	bgm_.LoadMP3("Music/stageSelect.mp3", "SelectBGM", bgmVolume_);

	//画像初期化
	for (int i = 0; i < _countTags; i++) {
		sp_[i] = std::make_unique<Sprite>(paths[i]);
	}

	gvu_ = new GlobalVariableUser("Scene", "Selects");

}

nSelectScene::~nSelectScene()
{
}

void nSelectScene::Initialize()
{
}

void nSelectScene::Update()
{
}

void nSelectScene::Draw()
{
}

void nSelectScene::SetGlobalV()
{
}

void nSelectScene::SceneChange()
{
}

void nSelectScene::SelectStage()
{
}
