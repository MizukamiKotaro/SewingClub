#pragma once
#include"Sprite.h"

#include"Camera.h"
#include"GlobalVariables/GlobalVariableUser.h"

#include<iostream>
#include<Vector3.h>
#include<list>

class UIGoalGuidance {

public:

	UIGoalGuidance();
	~UIGoalGuidance();

	//初期化
	void Initialize(const Vector3* playerPos, const Vector3* goalPos, const Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="playerPos">プレイヤー座標</param>
	/// <param name="goalPos">ゴール座標</param>
	/// <param name="camera">カメラ</param>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw();


	//ノルマの設定
	void SetQuota(const Vector3* position, const float size,const bool* isdead);
private:

	void Debug();

private:

	const Camera* camera_;

	const Vector3* playerPos_;

	const Vector3* goalPos_;

	//画像
	std::unique_ptr<Sprite>model_;

	GlobalVariableUser* gVUser_;

	Vector3 quotaColor_{ 1,1,1 };
	Vector3 goalColor_{1,1,1};

	enum Key {
		SpriteSize,
		SphereAreaSize,
		BoxAreaSize,
		AreaType,
		DirectionGoal,
		FadeOutUI,
		QuotaUIDirection,
		QuotaUISize,
		QuotaUIType,
		QuotaColor,
		GoalColor,
		_count
	};

	std::string keys[_count]={
		"スプライトサイズ",
		"円領域でのサイズ",
		"箱領域でのサイズ",
		"領域のタイプ(0で四角、1で円)",
		"ガイドのゴールとの最短距離",
		"ゴールが消えるまでのカウント",
		"ノルマとUIとの距離",
		"ノルマガイドUIのサイズ",
		"ノルマガイドUIのタイプ",
		"ノルマ案内の色",
		"ゴール案内の色"
	};

	//画像のサイズ
	Vector2 scale_ = { 64.0f,64.0f };

	//ゴールと→の距離
	float goalSize_ = 5.0f;

	//描画フラグ
	bool isDraw_=true;


	//円でのエリア範囲
	float sphereAreaSize_=1;

	//配置制限
	Vector2 area_ = { 1280,720 };

	enum AreaType
	{
		Squea,		//四角
		Sphere		//円
	};

	//制限エリアのタイプ
	int areaType_ = Squea;

	int fadeoutGoalCount_ = 0;
	//消えるまでのカウント
	int maxFadeoutGoalCount_ = 30;

	//ノルマ達成したか
	bool isQuota_;

	Vector2 quotaUISize_ = { 32,32 };

	struct QuotaData {
		std::unique_ptr<Sprite> sprite_;
		
		const Vector3* position_;
		float size_;
		const bool* isDead_;

		int quotaFadeoutGoalCount_ = 0;
		
	};

	//ノルマデータ
	std::list<QuotaData>quota_;

	//norma
	float quotaDirection_ = 10;
	int quotaAreaType_ = Sphere;
	//消えるまでのカウント
	int maxquotaFadeoutGoalCount_ = 30;
	
};
