#pragma once
#include"Sprite.h"
#include"Input.h"
#include"GlobalVariables/GlobalVariableUser.h"
#include"Vector3.h"
#include<iostream>
#include "GameElement/Animation/Animation2D.h"
#include"Audio.h"

struct ClearAnswer {
	bool goSelect = false;
	bool reTry = false;
	bool goNeext = false;
};

class GameClear {
public:

	GameClear();
	~GameClear();

	void Initialize(int stageNum,bool nextStageActive);

	ClearAnswer Update(const float& delta);

	void Draw();
	void SetBabyParam(const float& tension, const int& faceIndex);

private:

	void SetGlobalV();

	void InputUpdate();

	void ArrowUpdate();

	ClearAnswer ScceneChange();

	void TensionUpdate(const float& delta);

private:

	Input* input_;

	int stageNum_;

	float stickD_= 0.9f;
	bool inputActive_ = true;

	float alpha_ = 0.5f;
	Vector3 color_ = { 1,1,1 };
	Vector3 HUDColor_ = { 1,1,1 };
	Vector3 GageColor_ = { 1,1,1 };

	//次のステージがあるか否か
	bool isNextStage_ = true;

	
	enum SceneTy {
		kFadeIn,
		kPressButton,
		kInput,
		_countSceneTy
	};

	SceneTy nowS_;

	float aCount_ = 0;
	float amaxC_ = 60;

	// 赤ちゃん用アニメーション
	std::unique_ptr<Animation2D> babyAnimation_;
	// アニメーション必須変数
	float kMaxTensionPercent_ = 0.0f; // テンションの最大数。リザルトに渡される値
	float tensionPercent_ = 0.0f; // 足し続ける値
	Vector2 kMaxGaugePos_;
	Vector2 kMaxGaugeScale_;
	float nowFrame_ = 0.0f;
	const float kMaxAnimationFrame_ = 3.0f;

	Audio seMove_;
	Audio seSelect_;

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
		"ingame_select_HUD.png",
		
	};

	enum Results {
		kS,
		kA,
		kB,
		kC,
		_countResults
	};

	Results resultAns_;

	std::unique_ptr<Sprite>reTex_[_countResults];

	Vector2 rePos_ = { 640,360 };
	Vector2 reSize_ = {100,100};

	std::string rePaths_[_countResults] = {
		"gameClear_score_S.png",
		"gameClear_score_A.png",
		"gameClear_score_B.png",
		"gameClear_score_C.png",
	};
	
	struct AfterImage
	{
		std::unique_ptr<Sprite>sp;
		float count= 0;
		float maxAlpha;

		bool isDead = false;
	};

	float maxDC_ =30;

	//残像
	std::list<AfterImage>afterimage_;

	float afterICount_ = 0;
	float maxAfterC_ = 1;
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
		RePos,
		ReSize,
		_countAno
	};

	std::string anoKeys[_countAno] = {
		"背景透明度",
		"背景色",
		"HUD色",
		"動く速度",
		"動く量",
		"ゲージの色",
		"評価英 座標",
		"評価英 size",
	};
#pragma endregion

#pragma endregion

};