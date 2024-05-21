#include "AudioOptionUI.h"
#include"calc.h"

AudioOptionUI::AudioOptionUI()
{
	input_ = Input::GetInstance();

	VM_ = VolumeManager::GetInstance();

	
	for (int i = 0; i < _countOption; i++) {
		spSoundGageFrame_[i] = std::make_unique<Sprite>("soundGageFrame.png");
		spSoundGageBar_[i] = std::make_unique<Sprite>("soundGageBar.png");
		if (i == (int)BGMVolume) {
			spSoundText_[i] = std::make_unique<Sprite>("soundGageText.png");
		}
		else if (i == (int)SEVolume) {
			spSoundText_[i] = std::make_unique<Sprite>("SEsoundGageText.png");
		}
		spSoundGageNum_[i] = std::make_unique<Sprite>("soundGageFrameNum.png");
	}

	spNowSelectOption_ = std::make_unique<Sprite>("OptionSelect.png");

	pauseButton_ = std::make_unique<Sprite>("controler_UI_=-.png");
	backButton_ = std::make_unique<Sprite>("spBack.png");


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

#pragma region ImGui設定
	gVUser_ = new GlobalVariableUser("Option", "actionHelpUI");

	gVUser_->AddItem(keys[BGMGageFramePos], spSoundGageFrame_[BGMVolume]->pos_);
	gVUser_->AddItem(keys[BGMGageFrameSize], spSoundGageFrame_[BGMVolume]->size_);
	gVUser_->AddItem(keys[BGMGageBarPos], spSoundGageBar_[BGMVolume]->pos_);
	gVUser_->AddItem(keys[BGMGageBarSize], gageBarMaxScale_[BGMVolume]);
	gVUser_->AddItem(keys[BGMGageTextPos], spSoundText_[BGMVolume]->pos_);
	gVUser_->AddItem(keys[BGMGageTextSize], spSoundText_[BGMVolume]->size_);
	gVUser_->AddItem(keys[BGMGageNumPos], spSoundGageNum_[BGMVolume]->pos_);
	gVUser_->AddItem(keys[BGMGageNumSize], spSoundGageNum_[BGMVolume]->size_);

	gVUser_->AddItem(keys[SEGageFramePos], spSoundGageFrame_[SEVolume]->pos_);
	gVUser_->AddItem(keys[SEGageFrameSize], spSoundGageFrame_[SEVolume]->size_);
	gVUser_->AddItem(keys[SEGageBarPos], spSoundGageBar_[SEVolume]->pos_);
	gVUser_->AddItem(keys[SEGageBarSize], gageBarMaxScale_[SEVolume]);
	gVUser_->AddItem(keys[SEGageTextPos], spSoundText_[SEVolume]->pos_);
	gVUser_->AddItem(keys[SEGageTextSize], spSoundText_[SEVolume]->size_);
	gVUser_->AddItem(keys[SEGageNumPos], spSoundGageNum_[SEVolume]->pos_);
	gVUser_->AddItem(keys[SEGageNumSize], spSoundGageNum_[SEVolume]->size_);

	gVUser_->AddItem(keys[PauseButtonPos], pauseButton_->pos_);
	gVUser_->AddItem(keys[PauseButtonSize], pauseButton_->size_);
	gVUser_->AddItem(keys[backTextPos], backButton_->pos_);
	gVUser_->AddItem(keys[backTextSize], backButton_->size_);
#pragma endregion

}

AudioOptionUI::~AudioOptionUI()
{
}

void AudioOptionUI::Initialize()
{
	SetGlobalVData();
	

#pragma region 音関係


	spNowSelectOption_->Initialize();
#pragma endregion

}

bool AudioOptionUI::Update()
{
	SetGlobalVData();



	Vector2 input = input_->GetGamePadLStick();
	if (input.y <= -0.9f) {
		if (nowSelect == BGMVolume) {
			nowSelect = SEVolume;
			
		}
	}else if (input.y >= 0.9f) {
		if (nowSelect == SEVolume) {
			nowSelect = BGMVolume;
		}
	}

	spNowSelectOption_->pos_ = spSoundGageFrame_[(int)nowSelect]->pos_;

	AudioBarUpdate();

	spNowSelectOption_->Update();

	pauseButton_->Update();
	backButton_->Update();

	if (input_->PressedGamePadButton(Input::GamePadButton::START)) {

		return  false;
	}
	return true;
}

void AudioOptionUI::Draw()
{
	


	for (int i = 0; i < _countOption; i++) {
		//更新
		spSoundGageFrame_[i]->Draw();
		spSoundGageBar_[i]->Draw();
		spSoundText_[i]->Draw();
		spSoundGageNum_[i]->Draw();
	}

	spNowSelectOption_->Draw();

	pauseButton_->Draw();
	backButton_->Draw();
}

void AudioOptionUI::SetGlobalVData()
{
	spSoundGageFrame_[BGMVolume]->pos_ = gVUser_->GetVector2Value(keys[BGMGageFramePos]);
	spSoundGageFrame_[BGMVolume]->size_ = gVUser_->GetVector2Value(keys[BGMGageFrameSize]);
	spSoundGageBar_[BGMVolume]->pos_ = gVUser_->GetVector2Value(keys[BGMGageBarPos]);
	gageBarMaxScale_[BGMVolume] = gVUser_->GetFloatValue(keys[BGMGageBarSize]);
	spSoundText_[BGMVolume]->pos_ = gVUser_->GetVector2Value(keys[BGMGageTextPos]);
	spSoundText_[BGMVolume]->size_ = gVUser_->GetVector2Value(keys[BGMGageTextSize]);
	spSoundGageNum_[BGMVolume]->pos_ = gVUser_->GetVector2Value(keys[BGMGageNumPos]);
	spSoundGageNum_[BGMVolume]->size_ = gVUser_->GetVector2Value(keys[BGMGageNumSize]);

	spSoundGageFrame_[SEVolume]->pos_ = gVUser_->GetVector2Value(keys[SEGageFramePos]);
	spSoundGageFrame_[SEVolume]->size_ = gVUser_->GetVector2Value(keys[SEGageFrameSize]);
	spSoundGageBar_[SEVolume]->pos_ = gVUser_->GetVector2Value(keys[SEGageBarPos]);
	gageBarMaxScale_[SEVolume] = gVUser_->GetFloatValue(keys[SEGageBarSize]);
	spSoundText_[SEVolume]->pos_ = gVUser_->GetVector2Value(keys[SEGageTextPos]);
	spSoundText_[SEVolume]->size_ = gVUser_->GetVector2Value(keys[SEGageTextSize]);
	spSoundGageNum_[SEVolume]->pos_ = gVUser_->GetVector2Value(keys[SEGageNumPos]);
	spSoundGageNum_[SEVolume]->size_ = gVUser_->GetVector2Value(keys[SEGageNumSize]);

	pauseButton_->pos_ = gVUser_->GetVector2Value(keys[PauseButtonPos]);
	pauseButton_->size_ = gVUser_->GetVector2Value(keys[PauseButtonSize]);
	backButton_->pos_ = gVUser_->GetVector2Value(keys[backTextPos]);
	backButton_->size_ = gVUser_->GetVector2Value(keys[backTextSize]);
}

void AudioOptionUI::AudioBarUpdate()
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
