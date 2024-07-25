#pragma once
#include "SceneSystem/IScene/IScene.h"
#include"GameElement/SceneAcquisition/SceneAcquisition.h"


class GameOverScene : public IScene {

public:

	GameOverScene();
	~GameOverScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;


private:

	SceneAcquisition* sceneAcuition_;

};