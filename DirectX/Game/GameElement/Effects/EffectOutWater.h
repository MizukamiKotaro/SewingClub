#pragma once
#include "InstancingModelManager.h"
#include<list>

class EffectOutWater
{
public://シングルトンパターン
	static  EffectOutWater* GetInstance();

private://シングルトンパターン

	EffectOutWater() = default;
	~EffectOutWater() = default;
	EffectOutWater(const  EffectOutWater& o) = delete;
	const  EffectOutWater& operator=(const EffectOutWater& o) = delete;

public:

	//各準備
	void SetUp();

	//初期化
	void Initialize();

	void SetPlayerData(const Vector2& translate, const Vector2& Velo);

	//更新
	void Update();

	//描画
	void Draw();

	//終了処理
	void Finalize();

	/// <summary>
	/// エフェクト発生処理
	/// </summary>
	/// <param name="translate">生成開始座標</param>
	/// <param name="velo">プレイヤー進行ベクトルの向き</param>
	/// <param name="spawnNum">生成量</param>
	void SpawnEffect(const Vector2& translate, const Vector2& velo, int32_t spawnNum=10);

private:

	//ベクトル回転
	Vector2 RotateVelo(const Vector2& velo, float theta);

	bool isSetUp_ = false;

	//インスタンシングに必要
	InstancingModelManager* instancingManager_=nullptr;
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
		int32_t count =0;	//カウント
		Vector3 translate;	//座標
		float rotate=0;		//回転量
		float scale=0.1f;	//拡縮
		
		Vector3 velo;		//速度
		Vector3 acce;		//加速度

		bool isCountDead_ = true;
		bool isDead_ = false;
		int maxDeadCount_ = 30;

		float delayCount=0;
		float maxDelayCount = 5;

		bool isDraw=false;

		//残留エフェクトの表示間隔
		int spawnCount=0;
		int maxSpawnCount;
	};


	//泡データ
	std::list<std::unique_ptr<OutWaterData>>datas_;

	//データ
	std::list<std::unique_ptr<DustData>>dustDatas_;

	//プレイヤー向きベクトル
	const Vector2* playerVelo_;
	//プレイヤー座標
	const Vector2* playerTranslate_;


	//拡散角度
	float diffusionR_;

	//拡縮
	float scale_ = 0.1f;

	//最高速度
	float maxSpped_ = 10.0f;

	//遅延のカウント
	float spawnDelayCount_ = 5;

	//初速度の範囲x=min,y=max
	Vector2 startVelo = { 1.0f,1.5f };

	//初速度の細かい速度倍率
	float  rateScaling_ = 0.8f;

	//加速度の速度
	float acceSpd_ = 0.01f;

	//塵を出す感覚
	int spawnDustCount_ = 0;

	//生存カウントのプレイヤー速度に乗算する値
	float alliveLeverage_ = 90.0f;
};

