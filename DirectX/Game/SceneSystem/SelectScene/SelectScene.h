#pragma once

#include "SceneSystem/IScene/IScene.h"
#include"GameElement/BackGround/BackGround.h"
#include "Model.h"
#include"Audio.h"
#include "GameElement/Animation/Animation2D.h"
#include"GameElement/OptionUI/OptionUI.h"

class SelectScene : public IScene
{
public:
	SelectScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	//デバッグ
	void Debug();

	//シーン変更処理
	void SceneChange();

	//ステージを選ぶ処理
	void SelectStage();
	
private:

	//ステージ管理
	enum StageNumbers {
		ST1,
		ST2,
		ST3,
		_countOfStageNumbers	//数
	};

	//選択するステージボックス
	std::unique_ptr<Model>stageBoxes_[_countOfStageNumbers];

	//番号
	std::unique_ptr<Model>stageNumbers_[_countOfStageNumbers];

	//各ステージの配置データを持っておくところが欲しい

	//選ばれている番号
	int32_t pickedNum_ = 0;

	float ts_[_countOfStageNumbers];

	//切り替えをするときの変数軍
	struct  inputAcceptance {
		bool changeReception;	//操作を受け付けるかフラグ
		float moveLowerLimit;	//移動する際の最低量必須のスティック量
		float resetFlagLimit;	//フラグをリセットするための最低限必須な量	
		float maxSelectBoxScale_ = 1.5f;//選ばれているセレクトの最大スケール		
		float minSelectBoxScale_ = 1.0f;//最小スケール
		float maxScaleMinute = 1.0f / 30.0f;
		float minScaleMinute = 1.0f / 10.0f;
	};

	inputAcceptance switchData_;


	Audio bgm_;
	float bgmVolume_ = 0.15f;

	std::unique_ptr<Animation2D> animation_;

	// 背景
	std::unique_ptr<BackGround> bg_;

	//ボタン
	std::unique_ptr<Sprite> buttonA_;

	std::unique_ptr<Sprite> left_;
	std::unique_ptr<Sprite> right_;

	//Global用
	std::string groupName_ = "select";

	enum keys {
		buttonPos,
		buttonSize,
		leftPos,
		leftSize,
		rightPos,
		rightSize,
		_countKeys
	};

	std::string keys[_countKeys] = {
		"ボタン座標",
		"ボタンサイズ",
		"左矢印座標",
		"左矢印サイズ",
		"右矢印座標",
		"右矢印サイズ"
	};


	std::unique_ptr<OptionUI>optionUI_;
	bool isOptionActive_ = false;
	UpdateAnswer ans_;
};

