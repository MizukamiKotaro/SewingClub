#pragma once
#include"Sprite.h"
#include"Input.h"
#include"GlobalVariables/GlobalVariableUser.h"
#include"Vector3.h"
#include<iostream>
#include "GameElement/Animation/Animation2D.h"

struct ClearAnswer {
	bool goSelect = false;
	bool reTry = false;
	bool goNeext = false;
};

class GameClear {
public:

	GameClear();
	~GameClear();

	void Initialize(bool nextStageActive);

	ClearAnswer Update(const float& delta);

	void Draw();
	void SetBabyParam(const float& tension, const int& faceIndex);

private:

	void SetGlobalV();

	void InputUpdate();

	void ArrowUpdate();

	ClearAnswer ScceneChange();

private:

	Input* input_;

	float stickD_= 0.9f;
	bool inputActive_ = true;

	float alpha_ = 0.5f;
	Vector3 color_ = { 1,1,1 };
	Vector3 HUDColor_ = { 1,1,1 };
	Vector3 GageColor_ = { 1,1,1 };

	//次のステージがあるか否か
	bool isNextStage_ = true;

	// 赤ちゃん用アニメーション
	std::unique_ptr<Animation2D> babyAnimation_;

#pragma region 総画像管理
	//スプライト
	enum Text {
		Back,
		StageBackCloud,
		text_Stage,
		text_Num,
		text_Clear,	
		Hukidashi,
		Valuation_Normal,
		Valuation_Good,
		Valuation_Parfect,
		Gage_Bar_Frame,
		Gage_Bar,
		Baby,			//あかちゃん
		Arrow,
		Arrow_Frame,
		_countText
	};

	std::unique_ptr<Sprite>sp_[_countText] = { nullptr };

	//画像のパス
	std::string paths[_countText] = {
		"white.png",
		"Clear/ingame_gameclear_flame_HUD.png",
		"HUD/stage_HUD.png",
		"number_HUD.png",
		"HUD/ingame_gameClear_HUD.png",	
		"Clear/ingame_gameClear_baby_linesFlame.png",
		"HUD/ingame_gameClear_baby_normal_HUD.png",
		"HUD/ingame_gameClear_baby_good_HUD.png",
		"HUD/ingame_gameClear_baby_verygood_HUD.png",
		"baby_UI_gageDown.png",
		"baby_UI_gageDown.png",
		"baby_normal.png",		//赤ちゃん
		"HUD_arrow.png",
		"ingame_select_HUD.png"
	};


	
#pragma endregion



#pragma region 評価値管理
	enum Valuations {
		Normal,
		Good,
		Perfect,
		_countValuations
	};
	//評価
	Valuations valuation_ = Normal;

	//表情による赤ちゃんの差
	std::unique_ptr<Sprite>baby_[_countValuations];

	//差分画像
	std::string  babyPaths[_countValuations] = {
		"baby_normal.png",
		"baby_good.png",
		"baby_veryGood.png"
	};
#pragma endregion


#pragma region プレイヤー機能関係
	//プレイヤーの選択
	enum playerSelect {
		StageSelect,
		Retry,
		NextStage,
		_countPSelect
	};

	std::string selePath_[_countPSelect] = {
		"HUD/ingame_stageSelect_HUD.png",
		"HUD/ingame_retry_HUD.png",
		"HUD/ingame_gameClear_nextStage_HUD.png"
	};

	std::unique_ptr<Sprite>selects_[_countPSelect];

	Vector2 arrowPos_{};
	Vector2 animeMove_{};
	float count_ = 0;
	float swingSecond_ = 1.0f;
	float swingNum_ = 1.0f;

	playerSelect nowSelect_  = NextStage;
#pragma endregion

#pragma region ImGui関係
	GlobalVariableUser* gvu_ = nullptr;
#pragma region 総画像
	std::string spKeysP[_countText] = {
		"画面全体の色画像　座標",
		"クリアの後ろの雲　座標",
		"「ステージ」座標",
		"「番号」座標",
		"「クリア」座標",
		"吹き出し 座標",
		"評価 Normal 座標",
		"評価 Good 座標",
		"評価 Excerent 座標",
		"ゲージ枠 座標",
		"ゲージ 座標",
		"あかちゃん　座標",
		"矢印 座標",
		"選択物のHUD 座標"
	};
	std::string spKeysS[_countText] = {
		"画面全体の色画像　サイズ",
	"クリアの後ろの雲 サイズ",
	"「ステージ」サイズ",
	"「番号」サイズ",
	"「クリア」サイズ",
	"吹き出し サイズ",
	"評価 Normal サイズ",
	"評価 Good サイズ",
	"評価 Excerent サイズ",
	"ゲージ枠 サイズ",
	"ゲージ サイズ",
	"あかちゃん　サイズ",
	"矢印 サイズ",
	"選択物のHUD サイズ"
	};
#pragma endregion
#pragma region プレイヤー選択物
	std::string keysPseleP[_countPSelect] = {
		"ステージセレクト　座標",
		"リトライ　座標",
		"次のステージ　座標"
	};

	std::string keysPseleS[_countPSelect] = {
		"ステージセレクト　サイズ",
		"リトライ　サイズ",
		"次のステージ　サイズ"
	};
#pragma endregion
#pragma region その他
	enum anoTag {
		BackAlpha,
		BackColor,
		TextColor,
		SwingSecond,
		SwingNum,
		GageColor,
		_countAno
	};

	std::string anoKeys[_countAno] = {
		"背景透明度",
		"背景色",
		"HUD色",
		"動く速度",
		"動く量",
		"ゲージの色"
	};
#pragma endregion

#pragma endregion

	Vector2 kMaxGaugePos_;
	Vector2 kMaxGaugeScale_;

};