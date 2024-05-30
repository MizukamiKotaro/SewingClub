#pragma once
#include "SceneSystem/IScene/IScene.h"
#include "GameElement/BackGround/BackGround.h"
#include"GameElement/Effects/EffectUIEnterWater.h"
#include"GameElement/WaterEffect/WaterEffect.h"
#include"GameElement/OptionUI/OptionUI.h"
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
	void SetGlovalV();

	void LogoAnimation();

	//シーン変更処理
	void SceneChange();

private:
	std::string groupName_ = "title";

	std::unique_ptr<Sprite> buttonA_;

	std::unique_ptr<Sprite>select_;
	std::unique_ptr<Sprite>text_Option_;

#pragma region ロゴ関係
	std::unique_ptr<Sprite>titleLogo_[5];

	Vector2 logoPos_ = { 640,360 };
	Vector2 logos_[5];

	Vector2 logoCenters_[5];
	Vector2 logoMoveArea_ = { 5.0f,5.0f };

	Vector2 logoVelo_[5];
	Vector2 randVelo_ = {0.01f,0.05f};
#pragma endregion

	

	std::unique_ptr<Sprite>startWord_;
	// 背景
	std::unique_ptr<BackGround> bg_;

	Audio bgm_;
	float bgmVolume_ = 0.15f;

	//水の玉の円
	float waterWide_ = 10;

	std::unique_ptr<EffectUIEnterWater>effeUIEnterW_;

	std::unique_ptr<WaterEffect>waterE_;
	Vector3 color_;

	std::unique_ptr<Sprite>whiteS_;

#pragma region オプション関係
	std::unique_ptr<OptionUI>optionUI_;
	bool isOptionActive_ = false;
	UpdateAnswer ans_{};
#pragma endregion

};

