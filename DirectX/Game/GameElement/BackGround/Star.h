#pragma once
#include "Transform/Transform.h"

class Star {
public:
	Star() = default;
	~Star() = default;
	
	/// <summary>
	/// 初期化 のちに拡大率とかなんかいろいろやる
	/// </summary>
	/// <param name="position">出現座標</param>
	void Initialize(const Vector3& position);
	
	// true時に初期化処理を行うため
	bool Update();
	
	const Transform& GetTrans() const { return transform_; }
private:

	bool ScaleUpDown(); // とりあえず等速で拡縮
	bool isScaling_ = false; // true: 箱置きの変数。正直いらん
	Transform transform_;
	const float kMaxSize_ = 0.5f; // 最大サイズ
	
};

