#pragma once
#include "SceneSystem/IScene/IScene.h"
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

	int maxStageNum_ = 3;

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

	//画像のパス
	std::string paths[_countTags] = {

	};


	GlobalVariableUser* gvu_;
};