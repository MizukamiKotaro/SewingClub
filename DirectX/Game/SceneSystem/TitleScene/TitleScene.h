#pragma once
#include "SceneSystem/IScene/IScene.h"

class TitleScene : public IScene
{
public:
	TitleScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

	void WrightPostEffect();

private:

	//シーン変更処理
	void SceneChange();

private:

	std::unique_ptr<Sprite> buttonA_;
};

