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
	/// エフェクト発生フラグ
	/// </summary>
	/// <param name="isAllive"></param>
	void SpawnEffect(bool isAllive);

private:
	//インスタンシングに必要
	InstancingModelManager* instancingManager_ = nullptr;
	const InstancingMeshTexData* modelData_ = nullptr;

	//塵データ構造体
	struct Dust {
		Vector3 translate;
		Vector3 velo;

		//最大スケール
		float maxScale;

		float scale=0;

		bool isSizeUp = true;

		int count=0;
		int maxCount_;

		bool isDead_ = false;

		int tenmetuCount = 0;
		int maxTenmetu;
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
	float maxScale = 0.25f;

	//ランダム速度
	float randVelo =0.01f;

	//湧き間隔
	int spawnInterval_=0;

	int spawnIntervalCount_ = 0;

	//塵の拡縮一回までの時間
	int dustDeadCount_ = 10;

	//点滅量
	int maxTenmetuCount_ = 3;
};