#include "OptionUI.h"
#include<numbers>

OptionUI::OptionUI(SceneType type)
{
	input_ = Input::GetInstance();
	audioOption_ = std::make_unique<uAudioOptionUI>();

	backSprite_ = std::make_unique<Sprite>("white.png");
	backSprite_->size_ = { 1280,720 };
	blackFrame_ = std::make_unique<Sprite>("title_option_whitecloud.png");

	textPause_ = std::make_unique<Sprite>("HUD/ingame_pose_HUD.png");
	selectArrow_ = std::make_unique<Sprite>("HUD_arrow.png");
	textBackGame_ = std::make_unique<Sprite>("HUD/ingame_pose_backGame_HUD.png");
	textBackSelect_ = std::make_unique<Sprite>("HUD/ingame_stageSelect_HUD.png");
	textBackTitle_ = std::make_unique<Sprite>("HUD/ingame_stageSelect_title2_HUD.png");
	textSoundEdit_ = std::make_unique<Sprite>("HUD/option_musicVolume_HUD.png");
	textLeaveGame_ = std::make_unique<Sprite>("HUD/ingame_title_gameFinish_HUD.png");
	
	textPause_->SetColor({ 0,0,0,1 });
	//selectArrow_->SetColor({ 0,0,0,1 });
	textBackGame_->SetColor({ 0,0,0,1 });
	textBackSelect_->SetColor({ 0,0,0,1 });
	textBackTitle_->SetColor({ 0,0,0,1 });
	textSoundEdit_->SetColor({ 0,0,0,1 });
	textLeaveGame_->SetColor({ 0,0,0,1 });

	type_ = type;
	std::string groupName;

	switch (type)
	{
	case OptionUI::kTitle:
		groupName = "title";
		break;
	case OptionUI::kSelect:
		groupName = "Select";
		break;
	case OptionUI::kStage:
		groupName = "Stage";
		break;
	default:
		break;
	}
	gVUser_ = new GlobalVariableUser("Option", groupName);
	gVUser_->AddItem(keys[SpriteSelectArrow], arrowPosition_);
	gVUser_->AddItem(keys[SelectArrowSize], selectArrow_->size_);
	gVUser_->AddItem(keys[ArrowSwing], arrowSwingNum_);
	gVUser_->AddItem(keys[ArrowSwingCount], addSwingCount_);
	gVUser_->AddItem(keys[TextBackGamePos], textBackGame_->pos_);
	gVUser_->AddItem(keys[TextBackGameSize], textBackGame_->size_);
	gVUser_->AddItem(keys[TextBackSelectPos], textBackSelect_->pos_);
	gVUser_->AddItem(keys[TextBackSelectSize], textBackSelect_->size_);
	gVUser_->AddItem(keys[TextBackTitlePos], textBackTitle_->pos_);
	gVUser_->AddItem(keys[TextBackTitleSize], textBackTitle_->size_);
	gVUser_->AddItem(keys[TextSoundEditPos], textSoundEdit_->pos_);
	gVUser_->AddItem(keys[TextSoundEditSize], textSoundEdit_->size_);
	gVUser_->AddItem(keys[TextLeaveGamePos], textLeaveGame_->pos_);
	gVUser_->AddItem(keys[TextLeaveGameSize], textLeaveGame_->size_);
	gVUser_->AddItem(keys[BlackScreenAlpha], backalpha_);
	gVUser_->AddItem(keys[TextPause], textPause_->pos_);
	gVUser_->AddItem(keys[TextPauseSize], textPause_->size_);

}

OptionUI::~OptionUI()
{
}

void OptionUI::Initialize()
{
	
	backSprite_->SetColor({ 0, 0, 0, backalpha_ });
	audioOption_->Initialize();

	isStickBack_ = true;

	SetGrobalV();

	textBackGame_->Update();
	textBackSelect_->Update();
	textBackTitle_->Update();
	textSoundEdit_->Update();
	textLeaveGame_->Update();
}

void OptionUI::SetGrobalV()
{
	arrowPosition_ = gVUser_->GetVector2Value(keys[SpriteSelectArrow]);
	selectArrow_->size_ = gVUser_->GetVector2Value(keys[SelectArrowSize]);
	arrowSwingNum_ = gVUser_->GetFloatValue(keys[ArrowSwing]);
	addSwingCount_ = gVUser_->GetFloatValue(keys[ArrowSwingCount]);
	textBackGame_->pos_ = gVUser_->GetVector2Value(keys[TextBackGamePos]);
	textBackGame_->size_ = gVUser_->GetVector2Value(keys[TextBackGameSize]);
	textBackSelect_->pos_ = gVUser_->GetVector2Value(keys[TextBackSelectPos]);
	textBackSelect_->size_ = gVUser_->GetVector2Value(keys[TextBackSelectSize]);
	textBackTitle_->pos_ = gVUser_->GetVector2Value(keys[TextBackTitlePos]);
	textBackTitle_->size_ = gVUser_->GetVector2Value(keys[TextBackTitleSize]);

	textSoundEdit_->pos_ = gVUser_->GetVector2Value(keys[TextSoundEditPos]);
	textSoundEdit_->size_ = gVUser_->GetVector2Value(keys[TextSoundEditSize]);

	textLeaveGame_->pos_ = gVUser_->GetVector2Value(keys[TextLeaveGamePos]);
	textLeaveGame_->size_ = gVUser_->GetVector2Value(keys[TextLeaveGameSize]);
	backalpha_ = gVUser_->GetFloatValue(keys[BlackScreenAlpha]);

	textPause_->pos_ = gVUser_->GetVector2Value(keys[TextPause]);
	textPause_->size_ = gVUser_->GetVector2Value(keys[TextPauseSize]);


	backSprite_->SetColor({ 0, 0, 0, backalpha_ });

}

UpdateAnswer OptionUI::Update()
{
	ans_ = UpdateAnswer();

#ifdef _DEBUG
	SetGrobalV();
#endif // _DEBUG


	isBacked_ = false;
	backSprite_->Update();
	blackFrame_->Update();
	textPause_->Update();
	switch (type_)
	{
	case OptionUI::kTitle:
		TitleUpdate();
		break;
	case OptionUI::kSelect:
		SelectUpdate();
		break;
	case OptionUI::kStage:
		StageUpdate();
		break;
	default:
		break;
	}

	ArrowUpdate();




	
	//音量調整シーンから戻る処理
	if (opScene_ == kSound && !audioOption_->Update()) {
		opScene_ = kNone;
		nowSelect_ = BackGame;
		isBacked_ = true;
	}

	selectArrow_->Update();
	textBackGame_->Update();
	textBackSelect_->Update();
	textBackTitle_->Update();
	textSoundEdit_->Update();
	textLeaveGame_->Update();

	SceneChange();

	return ans_;
}

void OptionUI::Draw()
{
	//黒背景など描画
	backSprite_->Draw();
	blackFrame_->Draw();

	switch (type_)
	{
	case OptionUI::kTitle:
		TitleDraw();
		break;
	case OptionUI::kSelect:
		SelectDraw();
		break;
	case OptionUI::kStage:
		StageDraw();
		break;
	default:
		break;
	}

}

void OptionUI::SceneChange()
{
	//他シーンから戻ってきた場合通らない
	if (!isBacked_) {

		//ゲームに戻る処理
		if ((nowSelect_ == BackGame && input_->PressedGamePadButton(Input::GamePadButton::A)) || input_->PressedGamePadButton(Input::GamePadButton::START)) {
			ans_.backOption = true;
		}

		//タイトルシーンでの入力差
		if (type_ == kTitle) {

			//初期シーンの時
			if (opScene_ == kNone) {

				//音のセレクトに移動する処理
				if (nowSelect_ == SoundSelect && input_->PressedGamePadButton(Input::GamePadButton::A)) {
					opScene_ = kSound;
				}

				//ゲームをやめる処理
				if (nowSelect_ == LeaveGame && input_->PressedGamePadButton(Input::GamePadButton::A)) {
					ans_.leaveGame = true;
				}
			}

		}//セレクトシーンでの
		else if (type_ == kSelect) {
			//初期シーンの時
			if (opScene_ == kNone) {

				//音のセレクトに移動する処理
				if (nowSelect_ == SoundSelect && input_->PressedGamePadButton(Input::GamePadButton::A)) {
					opScene_ = kSound;
				}

				//タイトルに戻る処理
				if (nowSelect_ == BackTitle && input_->PressedGamePadButton(Input::GamePadButton::A)) {
					ans_.backtitle = true;
				}
			}
		}
		else if (type_ == kStage) {
			//初期シーンの時
			if (opScene_ == kNone) {


				//音のセレクトに移動する処理
				if (nowSelect_ == SoundSelect && input_->PressedGamePadButton(Input::GamePadButton::A)) {
					opScene_ = kSound;
				}

				//タイトルに戻る処理
				if (nowSelect_ == BackTitle && input_->PressedGamePadButton(Input::GamePadButton::A)) {
					ans_.backtitle = true;
				}

				//ステージセレクトに戻る処理
				if (nowSelect_ == BackSelect && input_->PressedGamePadButton(Input::GamePadButton::A)) {
					ans_.backSelect = true;
				}
			}
		}

	}

}



void OptionUI::ArrowUpdate()
{
#pragma region 矢印のY座標合わせ
	//選択したものに矢印のY座標を合わせる
	switch (nowSelect_)
	{
	case OptionUI::BackGame:
		selectArrow_->pos_.y = textBackGame_->pos_.y;

		break;
	case OptionUI::BackSelect:
		selectArrow_->pos_.y = textBackSelect_->pos_.y;

		break;
	case OptionUI::BackTitle:
		selectArrow_->pos_.y = textBackTitle_->pos_.y;

		break;
	case OptionUI::SoundSelect:
		selectArrow_->pos_.y = textSoundEdit_->pos_.y;

		break;
	case OptionUI::LeaveGame:
		selectArrow_->pos_.y = textLeaveGame_->pos_.y;

		break;
	default:
		break;
	}

	//ある値を超えると0に戻す
	swingCount_ += addSwingCount_/60.0f;
	swingCount_ = std::fmod(swingCount_, 2.0f * (float)std::numbers::pi);

	//サイン波のanimationの値を合わせたのを追加
	selectArrow_->pos_.x = arrowPosition_.x + std::sin(swingCount_) * arrowSwingNum_;
#pragma endregion
}

void OptionUI::TitleUpdate()
{
#pragma region 入力による現在選択しているもの変更

	//初期画面の場合の処理
	if (opScene_ == kNone) {
		//入力
		Vector2 move = input_->GetGamePadLStick();

		if (move.y >= -0.9f && move.y <= 0.9f) {
			isStickBack_ = true;
		}

		if (isStickBack_) {
			//タイトルには「戻る、音設定、ゲームをやめる」の三つ
			//上入力で選択を変更
			if (move.y >= 0.9f) {
				isStickBack_ = false;
				//各現在状態による次の選択への移動

				if (nowSelect_ == BackGame) {
					nowSelect_ = LeaveGame;
				}
				else if (nowSelect_ == SoundSelect) {
					nowSelect_ = BackGame;
				}
				else if (nowSelect_ == LeaveGame) {
					nowSelect_ = SoundSelect;
				}

			}//下入力で選択を変更
			else if (move.y <= -0.9f) {
				isStickBack_ = false;
				//各現在状態による次の選択への移動

				if (nowSelect_ == BackGame) {
					nowSelect_ = SoundSelect;
				}
				else if (nowSelect_ == SoundSelect) {
					nowSelect_ = LeaveGame;
				}
				else if (nowSelect_ == LeaveGame) {
					nowSelect_ = BackGame;
				}
			}
		}
	}

#pragma endregion

}

void OptionUI::SelectUpdate()
{
#pragma region 入力による現在選択しているもの変更

	//初期画面の場合の処理
	if (opScene_ == kNone) {
		//入力
		Vector2 move = input_->GetGamePadLStick();

		if (move.y >= -0.9f && move.y <= 0.9f) {
			isStickBack_ = true;
		}

		if (isStickBack_) {
			//タイトルには「戻る、音設定、ゲームをやめる」の三つ
			//上入力で選択を変更
			if (move.y >= 0.9f) {
				isStickBack_ = false;
				//各現在状態による次の選択への移動

				if (nowSelect_ == BackGame) {
					nowSelect_ = SoundSelect;
				}
				else if (nowSelect_ == BackTitle) {
					nowSelect_ = BackGame;
				}
				else if (nowSelect_ == SoundSelect) {
					nowSelect_ = BackTitle;
				}
				

			}//下入力で選択を変更
			else if (move.y <= -0.9f) {
				isStickBack_ = false;
				//各現在状態による次の選択への移動

				if (nowSelect_ == BackGame) {
					nowSelect_ = BackTitle;
				}
				else if (nowSelect_ == BackTitle) {
					nowSelect_ = SoundSelect;
				}
				else if (nowSelect_ == SoundSelect) {
					nowSelect_ = BackGame;
				}
				
			}
		}
	}

#pragma endregion
}

void OptionUI::StageUpdate()
{
#pragma region 入力による現在選択しているもの変更

	//初期画面の場合の処理
	if (opScene_ == kNone) {
		//入力
		Vector2 move = input_->GetGamePadLStick();

		if (move.y >= -0.9f && move.y <= 0.9f) {
			isStickBack_ = true;
		}

		if (isStickBack_) {
			//タイトルには「戻る、音設定、ゲームをやめる」の三つ
			//上入力で選択を変更
			if (move.y >= 0.9f) {
				isStickBack_ = false;
				//各現在状態による次の選択への移動

				if (nowSelect_ == BackGame) {
					nowSelect_ = SoundSelect;
				}
				else if (nowSelect_ == BackSelect) {
					nowSelect_ = BackGame;
				}
				else if (nowSelect_ == BackTitle) {
					nowSelect_ = BackSelect;
				}
				else if (nowSelect_ == SoundSelect) {
					nowSelect_ = BackTitle;
				}


			}//下入力で選択を変更
			else if (move.y <= -0.9f) {
				isStickBack_ = false;
				//各現在状態による次の選択への移動

				if (nowSelect_ == BackGame) {
					nowSelect_ = BackSelect;
				}
				else if (nowSelect_ == BackSelect) {
					nowSelect_ = BackTitle;
				}
				else if (nowSelect_ == BackTitle) {
					nowSelect_ = SoundSelect;
				}
				else if (nowSelect_ == SoundSelect) {
					nowSelect_ = BackGame;
				}

			}
		}
	}

#pragma endregion
}

void OptionUI::TitleDraw()
{
	switch (opScene_)
	{
	case OptionUI::kNone:


		textPause_->Draw();
		textBackGame_->Draw();
		textSoundEdit_->Draw();
		textLeaveGame_->Draw();

		selectArrow_->Draw();
		break;
	case OptionUI::kSound:

		audioOption_->Draw();

		break;
	case OptionUI::_countOption:
		break;
	default:
		break;
	}
}

void OptionUI::SelectDraw()
{
	switch (opScene_)
	{
	case OptionUI::kNone:


		textPause_->Draw();
		textBackGame_->Draw();
		textBackTitle_->Draw();
		textSoundEdit_->Draw();

		selectArrow_->Draw();
		break;
	case OptionUI::kSound:

		audioOption_->Draw();

		break;
	case OptionUI::_countOption:
		break;
	default:
		break;
	}
}

void OptionUI::StageDraw()
{
	switch (opScene_)
	{
	case OptionUI::kNone:


		textPause_->Draw();
		textBackGame_->Draw();
		textBackSelect_->Draw();
		textBackTitle_->Draw();
		textSoundEdit_->Draw();

		selectArrow_->Draw();
		break;
	case OptionUI::kSound:

		audioOption_->Draw();

		break;
	case OptionUI::_countOption:
		break;
	default:
		break;
	}
}
