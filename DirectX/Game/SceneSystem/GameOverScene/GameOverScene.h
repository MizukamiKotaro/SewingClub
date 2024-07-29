#pragma once
#include "SceneSystem/IScene/IScene.h"
#include"GameElement/SceneAcquisition/SceneAcquisition.h"
#include"GameElement/GameOver/GameOver.h"
#include"GameElement/SceneTransitionEffect/SceneTransitionEffect.h"


class GameOverScene : public IScene {

public:

	GameOverScene();
	~GameOverScene();

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

	//前シーン描画クラス
	SceneAcquisition* sceneAcuition_;

	//ゲームオーバー関係
	std::unique_ptr<GameOver>gameOver_;
	GameOverFlags gameOverFlags_;

	//遷移関係
	std::unique_ptr<SceneTransitionEffect>sceneTransition_;
	bool isDossolve_ = false;
};