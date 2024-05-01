#pragma once
#include"Sprite.h"
#include"GlobalVariables/GlobalVariableUser.h"

#include<list>
#include<iostream>

class EffectUIEnterWater
{

public:

	EffectUIEnterWater();
	~EffectUIEnterWater();

	void Initialize();

	void Update();

	void Draw();

	//シーン変更時処理
	void Finalize();

	void IsEffectActive(bool isActive) { isActive_ = isActive; };

private:
	void Debug();

	bool isActive_ = false;

	struct BubbleData {
		Sprite* sprite;
		Vector2 velo;
		float alpha=1;

		float deadCount=0;
		float maxDeadCount;

		bool isDead=false;

		
	};

	//泡データ
	std::list<std::unique_ptr<BubbleData>>datas_;

	GlobalVariableUser* gVariUser_;


	enum KeyTags
	{
		SpawnCount,//出現頻度
		SpawnDiff,//左右の出現範囲
		Size,//サイズ
		SizeRondom,//サイズのランダム度
		RandomVelo,//速度の最小最大
		DeadCount, //死亡までのカウント
		_count
	};


	std::string keys[_count]{
		"出現頻度",
		"左右の出現範囲",
		"サイズ",
		"サイズのランダム度",
		"速度のランダム",
		"死亡までのカウント"
	};

	//データの量
	int dataNum_ = 20;

	//出現間隔
	int spawnCount_ = 0;
	int maxSpawnCount_ = 5;

	//左右にエフェクト出現処理
	bool isSpawnLeft_ = true;

	//画面端から湧く範囲ランダム設定
	int spawnXDiff_ = 320;

	//サイズ
	float scale_ = 64.0f;
	
	//サイズランダム度（加算）
	float scaleDiff_ = 20.0f;

	//速度
	Vector2 veloDiff = { 10.0f,15.0f };

	//消えるまでのカウント数
	float maxDeadCount_=60;


};

