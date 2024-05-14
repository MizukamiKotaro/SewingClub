#include "OptionUI.h"
#include"calc.h"

OptionUI::OptionUI()
{
	input_ = Input::GetInstance();

	VM_ = VolumeManager::GetInstance();

	backSprite_ = std::make_unique<Sprite>("white.png");
	backSprite_->size_ = { 1280,720 };
	blackFrame_ = std::make_unique<Sprite>("title_option_whitecloud.png");

	for (int i = 0; i < _countOption; i++) {
		spSoundGageFrame_[i] = std::make_unique<Sprite>("soundGageFrame.png");
		spSoundGageBar_[i] = std::make_unique<Sprite>("soundGageBar.png");
		spSoundText_[i] = std::make_unique<Sprite>("soundGageText.png");
		spSoundGageNum_[i] = std::make_unique<Sprite>("soundGageFrameNum.png");
	}

}

OptionUI::~OptionUI()
{
}

void OptionUI::Initialize()
{
	backSprite_->SetColor({ 0, 0, 0, backalpha_ });

#pragma region 音関係
	for (int i = 0; i < _countOption; i++) {

		spSoundGageFrame_[i]->pos_.y += 100.0f * i;
		spSoundGageBar_[i]->pos_.y += 100.0f * i;
		spSoundText_[i]->pos_.y += 100.0f * i;
		spSoundGageNum_[i]->pos_.y += 100.0f * i;

		//一戸となりにずらす
		spSoundText_[i]->pos_.x -= spSoundText_[i]->size_.x * 1.0f;
		spSoundGageNum_[i]->pos_.y -= spSoundGageNum_[i]->size_.y * 1.0f;

		spSoundGageBar_[i]->SetAnchorPoint({ 0.0f,0.5f });
		spSoundGageBar_[i]->pos_.x -= spSoundGageBar_[i]->size_.x / 2;

		gageBarMaxScale_[i] = spSoundGageBar_[i]->size_.x;


	}
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



	for (int i = 0; i < _countOption; i++) {
		//更新
		spSoundGageFrame_[i]->Draw();
		spSoundGageBar_[i]->Draw();
		spSoundText_[i]->Draw();
		spSoundGageNum_[i]->Draw();
	}
}

void OptionUI::AudioBarUpdate()
{

	if (nowSelect == BGMVolume) {
		volume_[nowSelect] = VM_->GetMusicVolumeStage();
	}
	else if (nowSelect == SEVolume) {
		volume_[nowSelect] = VM_->GetSEVolumeStage();
	}
	
	

	//入力で音量変更
	Vector2 input = input_->GetGamePadLStick();

	



	if (input.x > 0) {
		volume_[nowSelect] += moveValue_;
	}
	else if (input.x < 0) {
		volume_[nowSelect] -= moveValue_;
	}
	if (volume_[nowSelect] < 0.0f) {
		volume_[nowSelect] = 0.0f;
	}
	else if (volume_[nowSelect] > 1.0f) {
		volume_[nowSelect] = 1.0f;
	}

	//volumeの値からサイズを変更
	float newScaleX = Calc::Lerp(0, gageBarMaxScale_[nowSelect], volume_[nowSelect]);
	spSoundGageBar_[nowSelect]->size_.x = newScaleX;

	if (nowSelect == BGMVolume) {
		VM_->SetMusicVolumeStage(volume_[nowSelect]);
	}
	else if (nowSelect == SEVolume) {
		VM_->SetSEVolumeStage(volume_[nowSelect]);
	}

	for (int i= 0; i < _countOption; i++) {
		//更新
		spSoundGageFrame_[i]->Update();
		spSoundGageBar_[i]->Update();
		spSoundText_[i]->Update();
		spSoundGageNum_[i]->Update();
	}
}
