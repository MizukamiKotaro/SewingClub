#pragma once
#include "SceneSystem/IScene/IScene.h"
#include"Audio.h"

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

	Audio bgm_;
	float bgmVolume_ = 0.15f;
};

