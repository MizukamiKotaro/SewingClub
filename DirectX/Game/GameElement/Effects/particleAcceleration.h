#pragma once
#include "InstancingModelManager.h"
#include"GlobalVariables/GlobalVariableUser.h"
#include"list"

class ParticleAcceleration {
public:

	ParticleAcceleration();
	~ParticleAcceleration();

	void Initialze(const Vector3*playerP,const float normalSpd, const float maxSpd);

	void Update(const Vector2& playerdirection,float deltaTime);

	void Draw();

	void IsActive(bool active,float spd);

private:

	void SetGlobalV();

	//インスタンシングに必要
	InstancingModelManager* instancingManager_ = nullptr;
	const InstancingMeshTexData* modelData_ = nullptr;

	//プレイヤー座標のポインタ
	const Vector3* playerPos_ = nullptr;

	//プレイヤー水中時での最大速度
	float pNormalMaxSpd_ = 0.0f;

	//プレイヤーの最大速度
	float pMaxSpd_ = 0.0f;

	//パーティクル関係
	struct ParticleData
	{
		Vector3 pos;
		Vector3 scale;
		Vector3 velo;
		float rotate;
		bool isDead = false;
		int count = 0;
		int dustSpawnCount = 0;
		int maxCount_;
		float alpha_;
		Vector3 color;
	};

	std::list<ParticleData>datas_;

	//残像エフェクト
	struct Dust
	{
		int count_ = 0;	//生存カウント
		
		Vector3 pos;
		Vector3 scale;
		float rotate;
		bool isDead = false;
		float alpha;
		
	};

	std::list<Dust>dDatas_;


#pragma region 各ImGuiデータ
	//処理のONOFF
	bool isActive_ = false;

	//発生間隔
	float spawnCount_ = 0;
	int maxSpawnCount_ = 5;

	//中心からどれだけ離れた点から出すか
	float directionCenter_ =1.0f;

	//プレイヤー速度に準じた沸き量
	float valueSpawnCount_ = 0;

	//影響度
	float pSpdpower_ = 1.0f;

	int dustDeadCount_ = 30;

	//パーティクル発生エリアサイズ
	Vector2 spawnAreaSize_ = { 10,5 };

	//透明度
	Vector2 stedAlpha_ = { 1,1 };

	//サイズ
	Vector2 stScale_ = { 1,1 };
	Vector2 edScale_ = { 0,0 };


	//生存カウント
	int maxDeadCount_ = 60;

	//ランダム速度方向
	Vector3 minVelo_ = { 0,1,0 };
	Vector3 maxVelo_ = { 0,1,0 };

	//ランダム速度
	Vector2 randSpd_ = { 0.1f,0.5f };

	Vector3 color_ = { 1,1,1 };

	//残像エフェクト生成頻度
	int maxDustSpawnCount_ = 1;

	Vector3 minColor_ = { 1,1,1 };
	Vector3 maxColor_ = { 0,0,0 };


	GlobalVariableUser* gVUser_=nullptr;
	//デバッグ用
	enum tags
	{
		SpawnCount,
		DirectionCenter,
		SpawnArea,
		StEdAlpha,
		StScale,
		EdScale,
		DeadCount,
		randSpd,
		color,
		DustSpawnCount,
		EffectivePspd,
		MinSpdColor,
		MaxSpdColor,
		_countNum
	};

	std::string keys[_countNum] = {
		"出現頻度",
		"沸き場所の中央からの差",
		"出現エリアサイズ",
		"最初と最後の透明度",
		"最初のサイズ",
		"最後のサイズ",
		"死亡カウント",
		"ランダム速度",
		"色",
		"塵の発生間隔",
		"プレイヤー速度影響度",
		"最小速度の色",
		"最大速度の色",

	};
#pragma endregion


};