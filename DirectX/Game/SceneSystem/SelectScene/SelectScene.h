#pragma once
#include "SceneSystem/IScene/IScene.h"
#include"GameElement/BackGround/BackGround.h"
#include"Audio.h"
#include"GameElement/OptionUI/OptionUI.h"
#include"GlobalVariables/GlobalVariableUser.h"


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
private:

	Input* input_=nullptr;

	//選ばれている番号(0,1,2~)
	int pickedNum_ = 0;

	const int maxStageNum_ = 8;

	// 背景
	std::unique_ptr<BackGround> bg_;

#pragma region Option関係
	std::unique_ptr<OptionUI>optionUI_;
	bool isOptionActive_ = false;
	UpdateAnswer ans_;
#pragma endregion

#pragma region 音関係
	Audio bgm_;
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


#pragma region 画像群
	//画像群
	enum spTags {
		Clound,
		text_Stage,
		Num1,
		Num10,
		LArrow,
		RArrow,
		Baby,
		_countTags
	};
	//画像
	std::unique_ptr<Sprite>sp_[_countTags];

	//画像のパス
	std::string paths[_countTags] = {
		"back_obj.png",
		"HUD/stage_HUD.png",
		"number_HUD.png",
		"number_HUD.png",
		"HUD_arrow.png",
		"HUD_arrow.png",
		"baby_normal.png",
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
		"uvChecker.png",
		"uvChecker.png",
		"uvChecker.png",
		"uvChecker.png",
		"uvChecker.png",
		"uvChecker.png",
		"uvChecker.png",
		"uvChecker.png",
		"uvChecker.png",
		"uvChecker.png",
		"uvChecker.png",
		"uvChecker.png",
		"uvChecker.png",
		"uvChecker.png",
		"uvChecker.png",
		"uvChecker.png",
	};
#pragma endregion

#pragma region ImGui関係
	GlobalVariableUser* gvu_;

	std::string spKeysP[_countTags] = {
		"雲　座標",
		"「ステージ」座標",
		"１桁目 座標 (触らない)",
		"２桁目　座標(触らない)",
		"左矢印　座標",
		"右矢印 座標",
		"赤ちゃん　座標",
	};
	std::string spKeysS[_countTags] = {
	"雲　サイズ",
	"「ステージ」サイズ",
	"１桁目 サイズ ",
	"２桁目　サイズ",
	"左矢印　サイズ",
	"右矢印 サイズ",
	"赤ちゃん　サイズ",
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
		MapPos,
		MapSize,
		_countAnother
	};

	std::string anoKeys[_countAnother] = {
		"数字の中心　座標",
		"二つの数字の距離",
		"矢印の速度",
		"矢印の振幅量",
		"マップ座標",
		"マップサイズ"
	};
#pragma endregion

};