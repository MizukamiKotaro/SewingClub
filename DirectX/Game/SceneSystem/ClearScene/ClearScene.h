#pragma once
#include "SceneSystem/IScene/IScene.h"
#include"GameElement/SceneAcquisition/SceneAcquisition.h"
#include"GameElement/GameClear/GameClear.h"
#include"GameElement/SceneTransitionEffect/SceneTransitionEffect.h"

class ClearScene : public IScene {

public:

	ClearScene();
	~ClearScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

	void FromBlackInitialize()override;
	void FromBlackUpdate()override;

	//シーン終了時遷移
	void ToBlackInitialize()override;
	void ToBlackUpdate()override;
private:

	void SceneChange();

	SceneAcquisition* sceneAcuition_;

	//クリア関係
	std::unique_ptr<GameClear>gameClear_;
	ClearAnswer gameClearFlags_;

	//ステージ最大数
	int maxStageNo_ = 8;

	//遷移関係
	std::unique_ptr<SceneTransitionEffect>sceneTransition_;
	bool isDossolve_ = false;
};