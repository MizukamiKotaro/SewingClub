#pragma once
#include"Sprite.h"
#include"Vector3.h"
#include"Camera.h"
#include<vector>
#include<iostream>


class UIEnemyGuidance {

public:
	~UIEnemyGuidance();

	void Initialize(const Camera* camera);

	void Update();

	void Draw();

	void SetEnemyPos(const Vector3& pos);

private:

	const Camera* camera_;

	struct Data {
		const Vector3* pos;
		Sprite*sp;
		float count_ = 0;
	};

	//データ群
	std::vector<Data*>datas_;

	//std::vector<std::unique_ptr<Sprite>>spData_;

	//領域
	Vector2 dangerline_ = { 400,200 };

	//透明処理開始エリア
	Vector2 fadeOutArea_ = { 1280,720 };

	//画像サイズ
	Vector2 scale_ = {64,64};

	//透明処理に使うカウント
	float maxCount_ = 30;


	//画像
	std::string path_ = "ingame_caveant.png";
};