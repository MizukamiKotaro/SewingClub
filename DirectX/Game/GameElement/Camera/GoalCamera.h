#pragma once
#include "Utils/Transform/Transform.h"
#include "Math/calc.h"
#include <memory>

class GoalCamera {
public:
	GoalCamera();
	~GoalCamera() = default;

	Vector3 Update(const Vector3& playerPos,const Vector3& goalPos, const float& delta);
	bool GetFinishd();

private:
	enum class TYPE {
		ToGoal, // playerからgoalへ
		Wait, // 待機中
		ToPlayer, // goalからplayerへ
		kMaxNum
	};
	TYPE type_ = TYPE::ToGoal;

	float count_ = 0.0f;
	float waitCount_ = 0.0f;
	const float kMaxFrame_ = 120.0f; // イージング用最大フレームカウント
	const float kWaitTime_ = 60.0f; // 一時停止待機時間
	
};
