#pragma once
#include"Sprite.h"
#include"GlobalVariables/GlobalVariableUser.h"
#include<iostream>
#include<list>
class EffectBabySleep {

public:

	EffectBabySleep();
	~EffectBabySleep();

	void Initialize();

	void Update(float delta);

	void Draw();

	void SetSpawnPos(const Vector2& pos);

private:

	void GlobalV();

	//エフェクト出現座標
	Vector2 spawnP_ = { 640,500 };

	//出現カウント
	float spCount_ = 0;
	float maxSpCount_ = 15.0f;

	//透明度の変更処理
	enum Mode {
		None,
		FadeOut,
		_countMode
	};

	struct Data {
		std::unique_ptr<Sprite>sp;
		Vector2 pos;
		Vector2 velo;
		float alpha;

		Mode mode = None;

		float count=0;
		bool isDead = false;

		float sinT = 0;
		Vector2 animeP = { 0,0 };
	};

	std::list<Data>datas_;

	//飛ばす向き
	Vector2 vector_{1,0};

	//速度
	float spd_ = 5.0f;

	//始終size
	Vector2 stedSize_ = { 0,50 };

	//透明度
	Vector2 stedAlpha_ = { 1,0 };

	//生存最大数
	float maxCount_ = 60.0f;

	//消え始めるカウント数
	float changeModeC_ = 30;

	//振幅速度
	float swingS_ =2.0f;
	//振幅幅
	float swingN_ = 8.0f;


#pragma region ImGui
	GlobalVariableUser* gvu_;

	enum tag {
		SPWait,
		Pos,
		Velo,
		Spd,
		Size,
		Alpha,
		MaxC,
		maxCC,
		SwingS,
		SwingN,
		_countT
	};
	std::string keys[_countT] = {
		"発生間隔",
		"発生座標",
		"飛んでいく方向",
		"速度",
		"始終サイズ",
		"終始透明度",
		"生存カウント",
		"透明始まりカウント",
		"振幅速度",
		"振幅量",
	};
#pragma endregion

};