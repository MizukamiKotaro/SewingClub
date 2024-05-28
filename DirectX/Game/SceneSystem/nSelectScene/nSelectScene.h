#pragma once
#include "SceneSystem/IScene/IScene.h"
#include"GameElement/BackGround/BackGround.h"
#include"Audio.h"
#include"GameElement/OptionUI/OptionUI.h"
#include"GlobalVariables/GlobalVariableUser.h"


class nSelectScene : IScene {
public:
	nSelectScene();
	~nSelectScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

	void SetGlobalV();

	void SceneChange();

	void SelectStage();

private:

	//最大ステージ数
	int maxStageNum_ = 3;

	//選ばれている番号
	int32_t pickedNum_ = 0;

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

#pragma region 画像群
	//画像群
	enum spTags {
		text_Stage,
		Num1,
		Num10,
		LArrow,
		RArrow,
		Clound,
		Baby,
		MapTex,
		_countTags
	};
	//画像
	std::unique_ptr<Sprite>sp_[_countTags];

	//画像のパス
	std::string paths[_countTags] = {
		"HUD/stage_HUD.png",
		"number_HUD.png",
		"number_HUD.png",
		"HUD_arrow.png",
		"HUD_arrow.png",
		"back_obj.png",
		"baby_normal.png",
		"white.png"
	};
#pragma endregion

#pragma region ImGui関係
	GlobalVariableUser* gvu_;
#pragma endregion

};