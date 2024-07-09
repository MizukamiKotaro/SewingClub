#pragma once
#include"InstancingModelManager.h"
#include"GlobalVariables/GlobalVariableUser.h"
#include<list>

class EffectGetItem {
public:

public:
	static EffectGetItem* GetInstance();
private:
	EffectGetItem() = default;
	~EffectGetItem() = default;
	EffectGetItem(const EffectGetItem& o) = delete;
	const EffectGetItem& operator=(const EffectGetItem& o) = delete;

public:

	void ModelLoad();

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void Spawn(const Vector3&  cPos);
private:

	void SetGlobalV();

private:
	//インスタンシングに必要
	InstancingModelManager* instancingManager_ = nullptr;
	const InstancingMeshTexData* modelData_ = nullptr;

	//
	struct DustData {
		Vector3 pos;
		Vector3 velo;
		float scale = 0;
		bool isDead = false;

		bool isSizeUp_ =true;//点滅処理のフラグ（trueでサイズアップ、falseで縮小)
		int count =0 ;

		int flashingCount = 0;
	};
	std::list<DustData>datas_;

	int maxCount_ = 60;
	float maxScale=1;
	int maxFlashing=3;

	Vector2 randSpd_ = { 0.5f,1.0f };
	float maxSpd_=1.0f;

	Vector2 randSpawnNum_ = { 2,3 };

	float gravity_ = 0.8f;
#pragma region データ
	GlobalVariableUser* gvu_;

	enum tags {
		kMaxCount,
		kMaxScale,
		kMaxFlash,
		kRandSpd,
		kMaxSpd,
		kRandSapwn,
		kGravity,
		_countTag
	};

	std::string keys[_countTag] = {
		"生存カウント",
		"最大サイズ",
		"点滅量",
		"ランダム速度（min,max",
		"最大速度",
		"ランダム生成量",
		"重力",
	};
#pragma endregion

};