#pragma once
#include "SceneSystem/IScene/IScene.h"
#include"GameElement/BackGround/BackGround.h"
#include"Audio.h"
#include"GameElement/OptionUI/OptionUI.h"
#include"GlobalVariables/GlobalVariableUser.h"
#include"GameElement/Effects/EffectBabySleep/EffectBabySleep.h"
#include"Engine/Drawers/PostEffect/Dissolve/Dissolve.h"
#include"GameElement/SceneTransitionEffect/SceneTransitionEffect.h"
#include"GameElement/WaterSurface/WaterSurface.h"

class SelectScene :public IScene {
public:
	SelectScene();
	~SelectScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

	void SetGlobalV();
	//シーン変更
	void SceneChange();
	//入力処理
	void InputUpdate();
	//矢印のアニメーション
	void ArrowUpdate();
	//番号の更新
	void NumberUpdate();
	//雲の動き更新
	void CloudUpdate();

	//画像更新
	void UpdateSprite();
private:

	Input* input_ = nullptr;

	//選ばれている番号(0,1,2~)
	int pickedNum_ = 0;

	const int maxStageNum_ = 8;

	// 背景
	std::unique_ptr<BackGround> bg_;

	std::unique_ptr<EffectBabySleep>effeBSleep_;
#pragma region Option関係
	std::unique_ptr<OptionUI>optionUI_;
	bool isOptionActive_ = false;
	UpdateAnswer ans_;
#pragma endregion

#pragma region 音関係
	Audio bgm_;
	Audio seOpenOption_;
	Audio seMove_;
	Audio seSelect_;
	float bgmVolume_ = 0.15f;
#pragma endregion

#pragma region 入力関係
	//数字の中心座標
	Vector2 numberCPos_ = { 640,360 };

	//二桁の時の数字同士の距離
	float numDistance_ = 10.0f;

	bool isInputActive_ = true;
	float inputDeadline_ = 0.9f;

	enum LR
	{
		Left,
		Right,
		_countLR
	};

	Vector2 arrowPos_[2]{};
	Vector2 animeMove_[2]{};
	float count_[2] = { 0 };
	float swingSecond_ = 1.0f;
	float swingNum_ = 1.0f;
#pragma endregion

	float bCount_ = { 0 };
	float bSwingSecond_ = 1.0f;
	float bSwingNum_ = 1.0f;
	Vector2 bAnimeP_ = { 0,0 };
#pragma region 雲の動き関係

	enum CloudState {
		None,
		Spawn1,
		Spawn2,
		Spawn3,
		FadeOut,
		_countState
	};
	bool isStateChange_ = true;
	CloudState state_ = FadeOut;

	bool isDraw_[_countState] = { false };
	float alpha_[_countState] = { 0 };

	int animeCount_ = 0;
	//各シーンカウント
	int maxAnimeCount_ = 60;

	Vector2 cPos_[_countState];
	Vector2 animePos_[_countState];
	float cCount_[_countState] = { 0 };
	
	float cSwingSeconds_[_countState];
#pragma endregion


#pragma region 画像群
	//画像群
	enum spTags {
		Clound,
		SmallClound1,
		SmallClound2,
		SmallClound3,
		text_Stage,
		Num1,
		Num10,
		LArrow,
		RArrow,
		BBody,
		Baby,
		Icon_Pause,
		Text_Pause,
		_countTags
	};
	//画像
	std::unique_ptr<Sprite>sp_[_countTags];

	//画像のパス
	std::string paths[_countTags] = {
		"outgame_stageSelect_flame.png",
		"outgame_stageSelect_HUD1.png",
		"outgame_stageSelect_HUD1.png",
		"outgame_stageSelect_HUD1.png",
		"HUD/stage_HUD.png",
		"number_HUD.png",
		"number_HUD.png",
		"HUD_arrow.png",
		"HUD_arrow.png",
		"Baby_Sleap.png",
		"outgame_stageSelect_baby.png",
		"controler_UI_=-.png",
		"ingame_pose_HUD.png",
	};

	enum Stages {
		kS1,
		kS2,
		kS3,
		kS4,
		kS5,
		kS6,
		kS7,
		kS8,
		kS9,
		kS10,
		kS11,
		kS12,
		kS13,
		kS14,
		kS15,
		kS16,
		_countStages
	};

	//マップ画像
	std::unique_ptr<Sprite>mapSprite_[_countStages];

	Vector2 mapPos_ = { 640,360 };
	Vector2 mapSize_ = { 100,100 };

	//画像パス
	std::string mapPaths_[_countStages] = {
		"maps/map1.png",
		"maps/map2.png",
		"maps/map3.png",
		"maps/map4.png",
		"maps/map5.png",
		"maps/map6.png",
		"maps/map7.png",
		"maps/map8.png",
		"maps/map8.png",
		"maps/map8.png",
		"maps/map8.png",
		"maps/map8.png",
		"maps/map8.png",
		"maps/map8.png",
		"maps/map8.png",
		"maps/map8.png",
	};
#pragma endregion

#pragma region ImGui関係
	GlobalVariableUser* gvu_;

	std::string spKeysP[_countTags] = {
		"雲　座標",
		"小さい雲1 座標",
		"小さい雲2 座標",
		"小さい雲3 座標",
		"「ステージ」座標",
		"１桁目 座標 (触らない)",
		"２桁目　座標(触らない)",
		"左矢印　座標",
		"右矢印 座標",
		"赤ちゃん体 座標",
		"赤ちゃん　座標",
		"ポーズアイコン 座標",
		"[ポーズ] 座標",
	};
	std::string spKeysS[_countTags] = {
	"雲　サイズ",
	"小さい雲1 サイズ",
	"小さい雲2 サイズ",
	"小さい雲3 サイズ",
	"「ステージ」サイズ",
	"１桁目 サイズ ",
	"２桁目　サイズ",
	"左矢印　サイズ",
	"右矢印 サイズ",
	"赤ちゃん体 サイズ",
	"赤ちゃん　サイズ",	
	"ポーズアイコン size",
	"[ポーズ] サイズ",
	};

	std::string mapKeysP[_countStages] = {
		"マップ1 座標",
		"マップ2 座標",
		"マップ3 座標",
		"マップ4 座標",
		"マップ5 座標",
		"マップ6 座標",
		"マップ7 座標",
		"マップ8 座標",
		"マップ9 座標",
		"マップ10 座標",

	};
	std::string mapKeysS[_countStages] = {
		"マップ1 サイズ",
		"マップ2 サイズ",
		"マップ3 サイズ",
		"マップ4 サイズ",
		"マップ5 サイズ",
		"マップ6 サイズ",
		"マップ7 サイズ",
		"マップ8 サイズ",
		"マップ9 サイズ",
		"マップ10 サイズ",

	};

	enum AnotherTag
	{
		NumPos,
		NumsDistance,
		SwingSecond,
		SwingNum,
		BSwingSecond,
		BSwingNum,
		MapPos,
		MapSize,
		AnimeCount,
		cS1,		
		cS2,		
		cS3,		
		cS4,
		
		SceneChangeNum,
		DissolveColor,
		DissolveDifference,

		_countAnother
	};

	std::string anoKeys[_countAnother] = {
		"数字の中心　座標",
		"二つの数字の距離",
		"矢印の速度",
		"矢印の振幅量",
		"赤ちゃんの振幅速度",
		"赤ちゃんの振幅量",
		"マップ座標",
		"マップサイズ",
		"アニメーションカウント",
		"雲1速度",
		"雲2速度",
		"雲3速度",
		"雲4速度",

		"シーン遷移速度",
		"Dissolveの差の色",
		"Dissolveの差の大きさ",
	};
#pragma endregion

	//各シーンチェンジ処理
	std::unique_ptr<SceneTransitionEffect>sceneTransition_;
	bool isChangeScene_ = false;

	std::unique_ptr<WaterSurface> surface_;
	std::unique_ptr<WaterSurface> surface2_;
};