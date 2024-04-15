#pragma once
#include"InstancingModelManager.h"

class  EffectExtraJump {

public:

	EffectExtraJump();
	~EffectExtraJump();

	void Initialize(const Vector3*playerPos);

	void Update();

	void Draw();

	void SpawnEffect(int count);

private:
	//インスタンシングに必要
	InstancingModelManager* instancingManager_ = nullptr;
	const ModelData* modelData_ = nullptr;

	struct Dust {
		Vector3 translate;
		Vector3 velo;

		int count=0;
		int maxCount_;

		bool isDead_ = false;
	};

	//塵データ
	std::list<std::unique_ptr<Dust>>datas_;

	//実行中か否か
	bool isActive_ = false;

	//発生カウント
	int count_ = 0;

	//プレイヤー座標保存
	const Vector3* playerPos_;

	//湧きエリアのサイズ
	Vector2 spawnAreaSize_ = { 0.5f,0.5f };

	//塵のサイズ
	Vector3 scale = { 0.05f,0.05f,1 };

	//湧き間隔
	int spawnInterval_=1;
	int spawnIntervalCount_ = 0;

	//塵の消える時間
	int dustDeadCount_ = 30;
};