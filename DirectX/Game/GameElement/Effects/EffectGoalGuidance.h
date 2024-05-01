#pragma once
#include"Sprite.h"
#include<iostream>
#include<Vector3.h>
#include"Camera.h"
#include"GlobalVariables/GlobalVariableUser.h"

class UIGoalGuidance {

public:

	UIGoalGuidance();
	~UIGoalGuidance();

	//初期化
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="playerPos">プレイヤー座標</param>
	/// <param name="goalPos">ゴール座標</param>
	/// <param name="goalSize">ゴールのサイズ</param>
	/// <param name="camera">カメラ</param>
	void Update(const Vector3& playerPos, const Vector3& goalPos, const float& goalSize,const Camera&camera);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera* camera);

private:

	void Debug();

	//画像
	std::unique_ptr<Sprite>model_;

	GlobalVariableUser* gVUser_;

	enum Key {
		SpriteSize,
		SphereAreaSize,
		BoxAreaSize,
		AreaType,
		_count
	};

	std::string keys[_count]={
		"スプライトサイズ",
		"円領域でのサイズ",
		"箱領域でのサイズ",
		"領域のタイプ"
	};

	//画像のサイズ
	Vector2 scale_ = { 64.0f,64.0f };

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
};
