#pragma once
#include "Utils/Transform/Transform.h"
#include "Math/calc.h"
#include <memory>

class GoalCamera {
public:
	GoalCamera() = default;
	~GoalCamera() = default;

	void Initialize();
	Vector3 Update(const Vector3& playerPos,const Vector3& goalPos, const float& delta);
	bool GetFinishd();
	uint32_t GetType() const { return static_cast<uint32_t>(type_); }
	void SetNext();

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
	// 単位 秒:second
	const float kMaxFrame_ = 1.5f; // イージング用最大フレームカウント
	const float kWaitTime_ = 0.5f; // 一時停止待機時間
	bool isSkiped = false;
	
};
