#pragma once
#include"Model.h"
#include"GlobalVariables/GlobalVariableUser.h"
#include "InstancingModelManager.h"
#include<list>

class EffectGoalDusts {
public:

	EffectGoalDusts();
	~EffectGoalDusts();

	void Initialize(const Vector3& gpos);

	void Update();

	void Draw();

	void SetActive(bool active);

private:

	void GetGlobalV();

	

private:

	bool isActive_ = false;

	const Vector3* goalPos_;

	//画像データ
	
	enum textureType {
		kT1,
		kT2,
		kT3,
		_countTexture
	};

	std::string dustPaths_[_countTexture] = {
		"ingame_goal_effect_circle.png",
		"ingame_goal_effect_cloud.png",
		"ingame_goal_effect_star.png"
	};

	

	//インスタンシングに必要
	InstancingModelManager* instancingManager_ = nullptr;
	const InstancingMeshTexData* modelData_[_countTexture] = { nullptr };

	struct DustData
	{
		textureType texType = kT1;
		bool IsDead_ = false;	//生存
		Vector3 pos;	//座標
		Vector3 velo;	//移動
		float rotate = 0;
		Vector3 size;	//サイズ
		Vector3 color;	//色
		float alpha;	//透明度

		float direction;//これとゴールの距離
		float maxDirection;//沸いたときの距離
	};

	std::list<DustData>datas_;
	
	float count_ = 0;
	float maxCount_ = 15;//出現間隔
	float preMaxCount = 30;

	bool randomRotate_ = true;
	bool randomColor_ = true;
	bool randomTexture_ = true;

	int selectTex = 0;

	//以下Imgui
	Vector2 randSpawnRadius_{ 3,5 };
	Vector2 randSpd_ = { 0.01f,0.05f };
	Vector2 stedSize_ = { 1.0f,0.0f };
	Vector2 stedAlpha_ = { 1,1 };

	GlobalVariableUser* gvu_ = nullptr;

	enum tags {
		MaxPreCount,
		MaxCount,
		IsRandomRotate,
		IsRandomColor,
		IsRandomTexture,
		SelectTex,
		RandSpawnRadius,
		RandSpd,
		StedSize,
		StedAlpha,
		_countTags
	};

	std::string keys[_countTags] = {
		"ゴール未出現カウント",
		"ゴール出現カウント",
		"ランダム角度処理",
		"ランダム色",
		"ランダム画像",
		"固定画像の時の画像",
		"ランダム出現範囲",
		"ランダム速度",
		"始終サイズ",
		"始終透明度"
	};
};
