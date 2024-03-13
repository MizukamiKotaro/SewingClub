#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "GameElement/Player/Player.h"

class TitleScene : public IScene
{
public:
	TitleScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

	void WrightPostEffect();

private:

};

