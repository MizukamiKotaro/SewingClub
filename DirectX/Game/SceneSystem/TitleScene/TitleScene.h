#pragma once
#include "SceneSystem/IScene/IScene.h"
#include "GameElement/BackGround/BackGround.h"
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
	std::string groupName_ = "title";

	std::unique_ptr<Sprite> buttonA_;
	std::unique_ptr<Sprite>titleLogo_;

	std::unique_ptr<Sprite>startWord_;
	// 背景
	std::unique_ptr<BackGround> bg_;

	Audio bgm_;
	float bgmVolume_ = 0.15f;
};

