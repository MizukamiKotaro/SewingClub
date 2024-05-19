#pragma once
#include "InstancingModelManager.h"
#include"GlobalVariables/GlobalVariableUser.h"
#include"list"

class ParticleAcceleration {
public:

	ParticleAcceleration();
	~ParticleAcceleration();

	void Initialze(const Vector3*playerP);

	void Update();

	void Draw();

	void IsActive(bool active);

private:

	//インスタンシングに必要
	InstancingModelManager* instancingManager_ = nullptr;
	const InstancingMeshTexData* modelData_ = nullptr;

	//プレイヤー座標のポインタ
	const Vector3* playerPos_;

	//パーティクル関係
	struct ParticleData
	{
		Vector3 pos;
		Vector3 scale;
		Vector3 velo;
		bool isDead = false;
		int count = 0;
		int maxCount_;
		float alpha_;
	};

	std::list<ParticleData>datas_;

#pragma region 各ImGuiデータ
	//処理のONOFF
	bool isActive_;

	//発生間隔
	int spawnCount_ = 0;
	int maxSpawnCount_ = 5;

	//パーティクル発生エリアサイズ
	Vector2 spawnAreaSize_ = { 10,5 };

	//透明度
	float stAlpha_ = 0;
	float edAlpha_ = 1;

	//サイズ
	float stScale_ = 1;
	float edScale_ = 0;

	//生存カウント
	int maxDeadCount_ = 60;

	//ランダム速度方向
	Vector3 minVelo_ = { 0,1,0 };
	Vector3 maxVelo_ = { 0,1,0 };

	//ランダム速度
	Vector2 randSpd_ = { 0.1f,0.5f };
#pragma endregion


};