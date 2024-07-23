#pragma once
#include "BasePostEffect/BasePostEffect.h"
#include "Vector3.h"
#include"Sprite.h"
#include"PostEffect/PostEffect.h"
#include"GameElement/OptionUI/OptionUI.h"

#include<iostream>

class  SceneAcquisition  {

public:

	static SceneAcquisition* GetInstance();
private:
	SceneAcquisition() = default;
	~SceneAcquisition() = default;
	SceneAcquisition(const SceneAcquisition& o) = delete;
	const SceneAcquisition& operator=(const SceneAcquisition& o) = delete;

public:

	//初期化
	void Initialize();

	//ここに描画されたものを描画
	void DrawScene();

	//各モデル書き込み前処理
	void PreDraw();

	//書き込み後処理
	void PostDraw();

	//終了処理
	void Finalize();

	//遷移前のシーン
	 OptionUI::SceneType PreSceneName_=OptionUI::SceneType::_countOfType;
private:

	std::unique_ptr<PostEffect>pe_;

};