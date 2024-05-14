#pragma once
#include"Input.h"
#include"Sprite.h"
#include"Audio/VolumeManager/VolumeManager.h"
#include<iostream>

//ポーズ画面の演出
class OptionUI {
public:

	OptionUI();
	~OptionUI();

	void Initialize();

	bool Update();

	void Draw();

private:

	void AudioBarUpdate();

private:

	Input* input_;
	VolumeManager* VM_;

	//背景スプライト
	std::unique_ptr<Sprite>backSprite_;
	//背景透過度
	float backalpha_ = 0.5f;

	//枠
	std::unique_ptr<Sprite>blackFrame_;


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
	//テキスト（音量
	std::unique_ptr<Sprite>spSoundText_[_countOption];
	//ゲージ上の数字
	std::unique_ptr<Sprite>spSoundGageNum_[_countOption];


	//ゲージの元サイズ保存
	float gageBarMaxScale_[_countOption] = { 0 };

	//音量(ここを変えてバー変更)
	float volume_[_countOption] = { 0.5f };

	//入力で変わる音量
	float moveValue_ = 0.01f;
#pragma endregion

};