#pragma once
#include"Input.h"
#include"Sprite.h"
#include"Audio/VolumeManager/VolumeManager.h"
#include"GlobalVariables/GlobalVariableUser.h"
#include<iostream>

//ポーズ画面の演出
class AudioOptionUI {
public:

	AudioOptionUI();
	~AudioOptionUI();

	void Initialize();

	bool Update();

	void Draw();

private:

	void SetGlobalVData();

	void AudioBarUpdate();

private:

	Input* input_;
	VolumeManager* VM_;
	

	


#pragma region 音設定関係

	enum OptionItem {
		BGMVolume,
		SEVolume,
		_countOption
	};

	OptionItem nowSelect = BGMVolume;

	//画像群
	//ゲージ枠
	std::unique_ptr<Sprite>spSoundGageFrame_[_countOption];
	//ゲージバー
	std::unique_ptr<Sprite>spSoundGageBar_[_countOption];
	//ゲージの元サイズ保存
	float gageBarMaxScale_[_countOption] = { 0 };
	//テキスト（音量
	std::unique_ptr<Sprite>spSoundText_[_countOption];
	//ゲージ上の数字
	std::unique_ptr<Sprite>spSoundGageNum_[_countOption];
	//現在選択しているものの表示
	std::unique_ptr<Sprite>spNowSelectOption_;


	//＝-ボタン
	std::unique_ptr<Sprite>pauseButton_;
	//戻るText
	std::unique_ptr<Sprite>backButton_;

	
	//音量(ここを変えてバー変更)
	float volume_[_countOption] = { 0.5f };

	//入力で変わる音量
	float moveValue_ = 0.01f;
#pragma endregion

#pragma region ImGui関係
	GlobalVariableUser* gVUser_;

	std::string tagName_ = "OptionUI";

	enum UIDatas {
		BGMGageFramePos,
		BGMGageFrameSize,
		BGMGageBarPos,
		BGMGageBarSize,
		BGMGageTextPos,
		BGMGageTextSize,
		BGMGageNumPos,
		BGMGageNumSize,

		SEGageFramePos,
		SEGageFrameSize,
		SEGageBarPos,
		SEGageBarSize,
		SEGageTextPos,
		SEGageTextSize,
		SEGageNumPos,
		SEGageNumSize,

		PauseButtonPos,
		PauseButtonSize,
		backTextPos,
		backTextSize,
		_count
	};

	std::string keys[UIDatas::_count] = {
		"BGM ゲージフレーム座標",
		"BGM ゲージフレームサイズ",
		"BGM ゲージバー座標",
		"BGM ゲージバーサイズ",
		"BGM テキスト座標",
		"BGM テキストサイズ",
		"BGM ゲージ上数字座標",
		"BGM ゲージ上数字サイズ",

		"SE ゲージフレーム座標",
		"SE ゲージフレームサイズ",
		"SE ゲージバー座標",
		"SE ゲージバーサイズ",
		"SE テキスト座標",
		"SE テキストサイズ",
		"SE ゲージ上数字座標",
		"SE ゲージ上数字サイズ",

		"一時停止ボタン座標",
		"一時停止ボタンサイズ",
		"「戻る」テキスト座標",
		"「戻る」テキストサイズ"
	};
#pragma endregion


};