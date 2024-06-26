#pragma once
#include "InstancingModelManager.h"
#include"GlobalVariables/GlobalVariableUser.h"
#include<list>

class EffectEnterWater {

public:

	EffectEnterWater(const std::string  name);
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

	

	/// <summary>
/// エフェクト発生処理
/// </summary>
/// <param name="translate">生成開始座標</param>
/// <param name="velo">プレイヤー進行ベクトルの向き</param>
/// <param name="spawnNum">生成量</param>
	void SpawnEffect(const Vector2& translate, const Vector2& velo, const Vector2& gpos);


private:
	void Debug();

	//ベクトル回転
	Vector2 RotateVelo(const Vector2& velo, float theta);

	//ラジアン変換
	float Radian(float digree);

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
	struct EnterWaterData
	{
		int32_t count = 0;	//カウント
		Vector3 translate;	//座標
		float rotate = 0;		//回転量
		float scale = 0.1f;	//拡縮

		float saveScale=0;

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
	std::list<std::unique_ptr<EnterWaterData>>datas_;

	//データ
	std::list<std::unique_ptr<DustData>>dustDatas_;

	enum keyTags {
		diffsion,//拡散量
		scale,//サイズ
		startVelo,//初速度のランダム範囲
		bendNum,//湾曲度
		maxSPD,//最高速度
		rateScaling,//速度の倍率
		spawnDustCount,//塵を出すまでのカウント
		acceSPD,//加速度の速度
		alliveLeverage,//生存カウントのプレイヤー速度に対する影響度
		setEqualSpace,//等間隔フラグ
		isRandomVelo,//速度ランダムフラグ
		SpawnNum,
		_count
	};

	std::string keys[_count] = {
		"拡散量",
		"サイズ",
		"初速度の範囲",
		"湾曲度",
		"最高速度",
		"初速度の倍率",
		"塵を出す間隔",
		"加速度の速度",
		"生存カウントのプレイヤー影響量",
		"等間隔に飛ばす",
		"速度をランダムにする",
		"出現量"
	};

	//拡散角度(弧の半分)（あとでpiかけているので注意）
	float diffusion_ = 1.0f / 4.0f;

	//オブジェクトの拡縮
	float scale_ = 0.1f;

	//初速度の範囲x=min,y=max
	Vector2 startVelo_ = { 1.0f,1.5f };

	//湾曲度
	float bendNum_ = 4.0f;

	//最高速度
	float maxSpped_ = 8.0f;

	//初速度の細かい速度倍率
	float  rateScaling_ = 1.0f;

	//塵を出す感覚
	int spawnDustCount_ = 1000;

	//遅延のカウント
	float spawnDelayCount_ = 0;


	//加速度の速度
	float acceSpd_ = 0.01f;

	//生存カウントのプレイヤー速度に対する影響度
	float alliveLeverage_ = 90.0f;

	//等間隔に角度を渡す
	bool setEqualSpace_ = false;

	//速度に差を作る
	bool isVeloRandom_ = false;

	int spawnNum_ = 5;

	GlobalVariableUser* gVariUser;
};

