#pragma once
#include"Engine/Drawers/PostEffect/Dissolve/Dissolve.h"
#include"Sprite.h"
#include"GlobalVariables/GlobalVariableUser.h"
#include<iostream>


class SceneTransitionEffect {
public:

	SceneTransitionEffect(const std::string&sceneName);
	~SceneTransitionEffect();

	void Initialize(float value=0.0f);

	//シーン開始時の遷移処理
	bool PreSceneTransition(float delta);

	//シーン終了時の遷移処理
	bool PostSceneTransition(float delta);

	void Debug();

	void Draw();

	void DrawPE();
private:

	void SetGlobalV();

private:

	GlobalVariableUser* gvu_=nullptr;



	std::unique_ptr<Sprite>dissolveBackTex_;
	//各シーンチェンジ処理
	bool preSceneChangeActive_ = false;
	bool postSceneChangeActive_ = false;
	//遷移速度
	float changeSecond_ = 1.0f;

	std::unique_ptr<Dissolve>dissolve_;

	Vector4 dissolveColor_ = { 1,1,1,1 };
	Vector4 edgeColor = { 1,1,1,1 };

	enum Tags {
		kLuminance,
		kChangeSpd,
		kDissolveColor,
		kEdgeColor,
		kDifference,
		_countOfTags
	};

	std::string keys[_countOfTags] = {
		"マスク量",
		"遷移速度",
		"色",
		"エッジ色",
		"色の差",
	};
};