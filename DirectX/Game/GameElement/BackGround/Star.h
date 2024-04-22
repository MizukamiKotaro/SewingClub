#pragma once
#include "Transform/Transform.h"

class Star {
public:
	Star() = default;
	~Star() = default;

	void Initialize(const Vector3& position);
	// true時に初期化処理を行うため
	bool Update();
	
	const Transform& GetTrans() const { return transform_; }
private:

	bool ScaleUpDown(); // とりあえず等速
	bool isScaling_ = false; // 箱置きの変数。正直いらん
	Transform transform_;
	const float kMaxSize_ = 0.5f; // 最大サイズ
	
};

