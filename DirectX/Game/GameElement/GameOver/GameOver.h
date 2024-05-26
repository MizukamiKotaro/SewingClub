#pragma once
#include"Sprite.h"
#include"GlobalVariables/GlobalVariableUser.h"
#include"Input.h"
#include"Vector3.h"

#include<iostream>

struct GameOverFlags {
	bool restart = false;
	bool goSelect = false;
};

class GameOver {
public:

	GameOver();
	~GameOver();

	void Initialize();

	GameOverFlags Update();

	void Draw();

private:

	void SetGlobalV();

	//シーンが変わる処理
	GameOverFlags SceneChange();

private:

	Input* input_;

	bool isInputMoveActive_ = true;

	float deadLineX_ = 0.9f;
	//黒背景
	std::unique_ptr<Sprite>back_;
	Vector3 backColor_={ 0,0,0 };
	float alpha_ =0.5f;
	//矢印6
	std::unique_ptr<Sprite>arrow_;
	Vector2 arrowPos_;
	Vector2 arrowAnime_;
	float swingSecond_ = 1.0f;
	float arrowSwing_ = 0.0f;
	float swingNum_ = 1.0f;

	float rotate_;

	enum Text {
		GA,
		AA,
		ME,
		O,
		Or,
		Ve,
		er,
		_countText
	};

	//ゲームオーバーの文字列
	std::unique_ptr<Sprite>GOText_[_countText];
	//各文字の中央座標
	Vector2 centerP_[_countText];
	//animationでの座標ずれ
	Vector2 movePos_[_countText];

	//移動
	Vector2 velo_[_countText];

	float rotates_[_countText];

	Vector2 allLogoPos_ = { 640,200 };
	float wordLength_ = 64.0f;

	Vector2 minMaxSpd_ = { 0.5f,1.0f };
	float maxDirection_ = 10.0f;

	float rotateNUm_ = 0.001f;

	enum SelectText {
		Retry,
		GoSelect,
		_countSelectText
	};

	//現在選択してるテキスト
	SelectText nowSelect_;

	//文字の画像
	std::unique_ptr<Sprite>text_[_countSelectText];

	//文字の下の画像
	std::unique_ptr<Sprite>backCloud_[_countSelectText];


#pragma region Global

	GlobalVariableUser* gVU_ = nullptr;

	enum Another {
		BackAlpha,
		BackColor,
		ArrowPos,
		ArrowSize,
		ArrowSecondSwing,
		SwingNum,
		LogoPos,
		LogoLength,
		MinMaxSpd,
		MaxDirection,
		RotateNum,
		_countAnother
	};

	std::string anoKeys[_countAnother] = {
		"黒背景透明度",
		"背景色",
		"矢印座標",
		"矢印サイズ",
		"矢印 何秒間隔で動くか",
		"矢印 動きの振幅",
		"「ゲームオーバー」全体座標",
		"「ゲームオーバー」文字の距離",
		"最小最大速度",
		"文字の移動できる最大距離",
		"回転量"
	};

	//テキストのサイズ
	std::string keysGOSize[_countText] = {
		"「ゲ」サイズ",
		"「ー」サイズ",
		"「ム」サイズ",
		"「オ」サイズ",
		"「ー」1サイズ",
		"「バ」サイズ",
		"「ー」2サイズ",
	};

	std::string paths[_countText] = {
		"gameOver/ingame_gameOver_ge.png",
		"gameOver/ingame_gameOver_-.png",
		"gameOver/ingame_gameOver_mu.png",
		"gameOver/ingame_gameOver_o.png",
		"gameOver/ingame_gameOver_-.png",
		"gameOver/ingame_gameOver_ba.png",
		"gameOver/ingame_gameOver_-.png"
	};
	
	//雲のサイズ
	std::string cKeysS[_countSelectText]={
		"リトライの雲　サイズ",
		"セレクトの雲　サイズ"
	};

	//文字画像の座標
	std::string textKeysP[_countSelectText] = {
		"リトライText　座標",
		"セレクトText　座標"
	};

	std::string textKeysS[_countSelectText] = {
		"リトライText　サイズ",
		"セレクトText　サイズ"
	};

#pragma endregion

};