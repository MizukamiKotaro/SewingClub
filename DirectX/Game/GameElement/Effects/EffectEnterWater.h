#pragma once
#include "InstancingModelManager.h"
#include<list>

class EffectEnterWater {

public:

	EffectEnterWater();
	~EffectEnterWater();


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	//ベクトル回転
	Vector2 RotateVelo(const Vector2& velo, float theta);


	//インスタンシングに必要
	InstancingModelManager* instancingManager_ = nullptr;
	const InstancingMeshTexData* modelData_ = nullptr;

	//残存エフェクト
	struct DustData {
		Vector3 translate;
		Vector3 velo;
		float scale;

		bool isDead = false;;

		float startScale;
		int count = 0;
		int maxCount;
	};

	int maxCount = 30;

	//描画データ群
	struct OutWaterData
	{
		int32_t count = 0;	//カウント
		Vector3 translate;	//座標
		float rotate = 0;		//回転量
		float scale = 0.1f;	//拡縮

		Vector3 velo;		//速度
		Vector3 acce;		//加速度

		bool isCountDead_ = true;
		bool isDead_ = false;
		int maxDeadCount_ = 30;

		float delayCount = 0;
		float maxDelayCount = 5;

		bool isDraw = false;

		//残留エフェクトの表示間隔
		int spawnCount = 0;
		int maxSpawnCount;
	};


	//泡データ
	std::list<std::unique_ptr<OutWaterData>>datas_;

	//データ
	std::list<std::unique_ptr<DustData>>dustDatas_;



	//拡散角度(弧の半分)（あとでpiかけているので注意）
	float diffusionR_ = 1.0f / 2.0f;

	//オブジェクトの拡縮
	float scale_ = 0.1f;

	//初速度の範囲x=min,y=max
	Vector2 startVelo = { 1.0f,1.5f };

	//湾曲度
	float bendNum_ = 2.0f;


};