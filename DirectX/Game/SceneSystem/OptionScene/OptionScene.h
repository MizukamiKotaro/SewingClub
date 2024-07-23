#pragma once
#include "SceneSystem/IScene/IScene.h"
#include"GameElement/SceneAcquisition/SceneAcquisition.h"
#include"GameElement/OptionUI/OptionUI.h"


class OptionScene : public IScene {

public:

	OptionScene();
	~OptionScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;


private:

	SceneAcquisition* sceneAcuition_;
	std::unique_ptr<OptionUI>optionUI_;

};