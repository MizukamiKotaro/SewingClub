#pragma once
#include<GameElement/OptionUI/AudioOptionUI.h>


class OptionUI {

public:

	enum SceneType {
		kTitle,
		kSelect,
		kStage
	};

	OptionUI();
	~OptionUI();

	void Initialize(SceneType type);

	bool Update();

	void Draw();

private:

	//背景スプライト
	std::unique_ptr<Sprite>backSprite_;
	//背景透過度
	float backalpha_ = 0.5f;

	//枠
	std::unique_ptr<Sprite>blackFrame_;

	//シーンタイプ取得
	SceneType type_;

	std::unique_ptr<AudioOptionUI> audioOption_;
};