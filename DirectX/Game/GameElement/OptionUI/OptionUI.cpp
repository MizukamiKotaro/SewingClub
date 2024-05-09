#include "OptionUI.h"
#include"calc.h"

OptionUI::OptionUI()
{
	input_ = Input::GetInstance();

	backSprite_ = std::make_unique<Sprite>("white.png");
	backSprite_->size_ = { 1280,720 };
	blackFrame_ = std::make_unique<Sprite>("title_option_whitecloud.png");

	spSoundGageFrame_ = std::make_unique<Sprite>("soundGageFrame.png");
	spSoundGageBar_ = std::make_unique<Sprite>("soundGageBar.png");
	spSoundText_ = std::make_unique<Sprite>("soundGageText.png");
	spSoundGageNum_ = std::make_unique<Sprite>("soundGageFrameNum.png");
}

OptionUI::~OptionUI()
{
}

void OptionUI::Initialize()
{
	backSprite_->SetColor({ 0, 0, 0, backalpha_ });
	
#pragma region 音関係
	
	//一戸となりにずらす
	spSoundText_->pos_.x -= spSoundText_->size_.x * 1.0f;
	spSoundGageNum_->pos_.y -= spSoundGageNum_->size_.y * 1.0f;

	spSoundGageBar_->SetAnchorPoint({ 0.0f,0.5f });
	spSoundGageBar_->pos_.x -= spSoundGageBar_->size_.x/2;

	gageBarMaxScale_ = spSoundGageBar_->size_.x;
#pragma endregion

}

bool OptionUI::Update()
{
	backSprite_->Update();
	backSprite_->Update();


	AudioBarUpdate();

	

	if (input_->PressedGamePadButton(Input::GamePadButton::START)) {
		return  false;
	}
	return true;
}

void OptionUI::Draw()
{
	//黒背景など描画
	backSprite_->Draw();
	blackFrame_->Draw();

	spSoundGageFrame_->Draw();
	spSoundGageBar_->Draw();
	spSoundText_->Draw();
	spSoundGageNum_->Draw();
}

void OptionUI::AudioBarUpdate()
{

	//volumeの値からサイズを変更
	float newScaleX = Calc::Lerp(0, gageBarMaxScale_, volume_);
	spSoundGageBar_->size_.x = newScaleX;

	//更新
	spSoundGageFrame_->Update();
	spSoundGageBar_->Update();
	spSoundText_->Update();
	spSoundGageNum_->Update();
}
