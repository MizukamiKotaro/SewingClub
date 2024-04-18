#pragma once
#include"InstancingModelManager.h"

class  EffectExtraJump {

public:

	EffectExtraJump();
	~EffectExtraJump();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="playerPos">プレイヤー座標</param>
	void Initialize(const Vector3*playerPos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ作業
	/// </summary>
	void Debug();

	/// <summary>
	/// エフェクト発生
	/// </summary>
	/// <param name="count">発生量</param>
	void SpawnEffect(int count);

private:
	//インスタンシングに必要
	InstancingModelManager* instancingManager_ = nullptr;
	const InstancingMeshTexData* modelData_ = nullptr;

	//塵データ構造体
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