#pragma once
#include "SceneSystem/IScene/IScene.h"
#include"GameElement/SceneAcquisition/SceneAcquisition.h"


class ClearScene : public IScene {

public:

	ClearScene();
	~ClearScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;


private:

	SceneAcquisition* sceneAcuition_;

};