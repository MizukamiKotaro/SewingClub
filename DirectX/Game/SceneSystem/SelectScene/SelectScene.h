#pragma once

#include "SceneSystem/IScene/IScene.h"


class SelectScene : public IScene
{
public:
	SelectScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

	//シーン変更処理
	void SceneChange();

private:
	


};

