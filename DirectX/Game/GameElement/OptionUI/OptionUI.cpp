#include "OptionUI.h"

OptionUI::OptionUI()
{
	audioOption_ = std::make_unique<AudioOptionUI>();

	backSprite_ = std::make_unique<Sprite>("white.png");
	backSprite_->size_ = { 1280,720 };
	blackFrame_ = std::make_unique<Sprite>("title_option_whitecloud.png");

}

OptionUI::~OptionUI()
{
}

void OptionUI::Initialize(SceneType type)
{
	type_ = type;
	audioOption_->Initialize();
}

bool OptionUI::Update()
{
	backSprite_->SetColor({ 0, 0, 0, backalpha_ });

	backSprite_->Update();

	audioOption_->Update();
	return true;
}

void OptionUI::Draw()
{
	//黒背景など描画
	backSprite_->Draw();
	blackFrame_->Draw();

	audioOption_->Draw();
}
