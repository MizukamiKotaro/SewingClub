#pragma once
#include"Input.h"
#include"Sprite.h"

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

	//背景スプライト
	std::unique_ptr<Sprite>backSprite_;
	//背景透過度
	float backalpha_ = 0.5f;

	//枠
	std::unique_ptr<Sprite>blackFrame_;


#pragma region 音設定関係
	//画像群
	//ゲージ枠
	std::unique_ptr<Sprite>spSoundGageFrame_;
	//ゲージバー
	std::unique_ptr<Sprite>spSoundGageBar_;
	//テキスト（音量
	std::unique_ptr<Sprite>spSoundText_;
	//ゲージ上の数字
	std::unique_ptr<Sprite>spSoundGageNum_;

	//ゲージの元サイズ保存
	float gageBarMaxScale_ = 0;

	//音量(ここを変えてバー変更)
	float volume_ = 0.5f;
#pragma endregion

};