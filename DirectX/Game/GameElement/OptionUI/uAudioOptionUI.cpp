#include"uAudioOptionUI.h"
#include"calc.h"


uAudioOptionUI::uAudioOptionUI()
{

	VM_ = VolumeManager::GetInstance();


	input_ = Input::GetInstance();

	for (int type = 0; type < _countVolumeType; type++) {
		for (int sp = 0; sp < _countSprite; sp++) {
			gage_[type][sp] = std::make_unique<Sprite>(paths[type][sp]);
		}

		gage_[type][Gage]->SetAnchorPoint({ 0.0f,0.5f });
	}

	arrow_ = std::make_unique<Sprite>("HUD_arrow.png");

	text_volume_ = std::make_unique<Sprite>("option_musicVolume_HUD.png");

	gvu_ = new GlobalVariableUser("Option", "Audio");
	for (int type = 0; type < _countVolumeType; type++) {
		for (int sp = 0; sp < _countSprite; sp++) {
			gvu_->AddItem(keysP[type][sp], gage_[type][sp]->pos_);
			gvu_->AddItem(keysS[type][sp], gage_[type][sp]->size_);
		}
	}

	gvu_->AddItem(anoKeys[ArrowPos], aPos_);
	gvu_->AddItem(anoKeys[ArrowSize], arrow_->size_);
	gvu_->AddItem(anoKeys[TextVolumePos], text_volume_->pos_);
	gvu_->AddItem(anoKeys[TextVolumeSize], text_volume_->size_);

	for (int i = 0; i < _countColors; i++) {
		gvu_->AddItem(colorKey_[i], colors_[i]);
	}
}

void uAudioOptionUI::SetGlobalVData()
{
	for (int type = 0; type < _countVolumeType; type++) {
		for (int sp = 0; sp < _countSprite; sp++) {

			gage_[type][sp]->pos_ = gvu_->GetVector2Value(keysP[type][sp]);
			if (sp == Gage) {
				maxGageScale_[type] = gvu_->GetVector2Value(keysS[type][sp]);
			}
			else {
				gage_[type][sp]->size_ = gvu_->GetVector2Value(keysS[type][sp]);
			}

		}
	}

	aPos_ = gvu_->GetVector2Value(anoKeys[ArrowPos]);
	arrow_->size_ = gvu_->GetVector2Value(anoKeys[ArrowSize]);
	text_volume_->pos_ = gvu_->GetVector2Value(anoKeys[TextVolumePos]);
	text_volume_->size_ = gvu_->GetVector2Value(anoKeys[TextVolumeSize]);

	for (int i = 0; i < _countColors; i++) {
		colors_[i] = gvu_->GetVector3Value(colorKey_[i]);
	}

	text_volume_->SetColor({ colors_[Text].x,colors_[Text].y,colors_[Text].z,1 });

	volume_[BGM] = VM_->GetMusicVolumeStage();
	volume_[SE] = VM_->GetSEVolumeStage();
}

uAudioOptionUI::~uAudioOptionUI()
{
}

void uAudioOptionUI::Initialize()
{
	SetGlobalVData();

	UpdateSprite();
}

bool uAudioOptionUI::Update()
{

#ifdef _DEBUG
	SetGlobalVData();
#endif // _DEBUG

#pragma region 入力による選択物変化
	Vector2 move = input_->GetGamePadLStick();

	if (isInputActive_) {
		//上入力
		if (move.y > inputDline_) {
			isInputActive_ = false;
			if (nowSelect_ == BGM) {
				nowSelect_ = Back;
			}
			else if (nowSelect_ == SE) {
				nowSelect_ = BGM;
			}
			else if (nowSelect_ == Back) {
				nowSelect_ = SE;
			}

		}//下入力
		else if (move.y < -inputDline_) {
			isInputActive_ = false;
			if (nowSelect_ == BGM) {
				nowSelect_ = SE;
			}
			else if (nowSelect_ == SE) {
				nowSelect_ = Back;
			}
			else if (nowSelect_ == Back) {
				nowSelect_ = BGM;
			}
		}
	}
	else {
		if (move.y<inputDline_ && move.y>-inputDline_) {
			isInputActive_ = true;
		}
	}
#pragma endregion

	AudioBarUpdate();

#pragma region 矢印
	arrow_->pos_ = gage_[nowSelect_][Gage]->pos_ + aPos_ + animeA_;
#pragma endregion


	UpdateSprite();


	if (input_->PressedGamePadButton(Input::GamePadButton::START) ||
		(nowSelect_ == Back && input_->PressedGamePadButton(Input::GamePadButton::A))) {

		return false;
	}

	return true;
}

void uAudioOptionUI::Draw()
{
	for (int type = 0; type < _countVolumeType; type++) {
		for (int sp = 0; sp < _countSprite; sp++) {
			if (type == Back) {
				if (sp != Gage) {
					gage_[type][sp]->Draw();
				}
			}
			else {
				gage_[type][sp]->Draw();
			}
		}
	}
	text_volume_->Draw();
	arrow_->Draw();
}



void uAudioOptionUI::AudioBarUpdate()
{

	if (nowSelect_ == Back) {
		return;
	}

	//各値取得
	volume_[BGM] = VM_->GetMusicVolumeStage();
	volume_[SE] = VM_->GetSEVolumeStage();

	//入力で音量変更
	Vector2 input = input_->GetGamePadLStick();

	//入力可能時操作
	if (isInputActive_) {
		//現在選択しているほうの値を変更
		if (input.x > inputDline_) {
			volume_[nowSelect_] += moveValue_;
		}
		else if (input.x < -inputDline_) {
			volume_[nowSelect_] -= moveValue_;
		}
	}

	//領域外対策
	if (volume_[nowSelect_] < 0.0f) {
		volume_[nowSelect_] = 0.0f;
	}
	else if (volume_[nowSelect_] > 1.0f) {
		volume_[nowSelect_] = 1.0f;
	}

	//変更した音を追加
	if (nowSelect_ == BGM) {
		VM_->SetMusicVolumeStage(volume_[nowSelect_]);
	}
	else if (nowSelect_ == SE) {
		VM_->SetSEVolumeStage(volume_[nowSelect_]);
	}

	VM_->SaveVolumeStage(volume_[SE], volume_[BGM]);

#pragma region 選択物の色変更処理
	for (int type = 0; type < _countVolumeType; type++) {
		if (nowSelect_ == type) {
			gage_[type][Frame]->SetColor({ colors_[SelectFrame].x,colors_[SelectFrame].y,colors_[SelectFrame].z,1 });
			gage_[type][Gage]->SetColor({ colors_[SelectGage].x,colors_[SelectGage].y,colors_[SelectGage].z,1 });
		}
		else {
			gage_[type][Frame]->SetColor({ colors_[NonSelectFrame].x,colors_[NonSelectFrame].y,colors_[NonSelectFrame].z,1 });
			gage_[type][Gage]->SetColor({ colors_[NonSelectGage].x,colors_[NonSelectGage].y,colors_[NonSelectGage].z,1 });
		}

	}
#pragma endregion


}

void uAudioOptionUI::UpdateSprite()
{
	text_volume_->Update();
	arrow_->Update();

	gage_[BGM][Gage]->size_ = Calc::Lerp({ 0,maxGageScale_[BGM].y}, maxGageScale_[BGM], volume_[BGM]);
	gage_[SE][Gage]->size_ = Calc::Lerp({ 0,maxGageScale_[SE].y }, maxGageScale_[SE], volume_[SE]);

	for (int type = 0; type < _countVolumeType; type++) {
		for (int sp = 0; sp < _countSprite; sp++) {
			gage_[type][sp]->Update();
		}
	}
}
