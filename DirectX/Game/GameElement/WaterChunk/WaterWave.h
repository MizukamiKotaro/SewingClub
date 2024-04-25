#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include <string>
#include <memory>
#include <vector>
#include <list>
#include "GlobalVariables/GlobalVariableUser.h"

class WaterWave {
public:
	WaterWave(const Vector3& velocity, const float& rotate, const bool& isDown, const int& no);

	static void StaticInitialize();

	void Update(const float& deltaTime);

	static void StaticUpdate();

	bool IsFinish() const;

	float GetPower(const float& rotate) const;

	const std::list<int>& GetNums() const { return nums_; }

private:
	static void SetGlobalVariable();

	static void ApplyGlobalVariable();

private:
	enum FloatParamater {
		kConvertingMagnification, // 力が最大の時に動く最大の角度
		kExpandTime, // 広がりきる時間
		kMaxSpeed, // 力が最大になるときの速度
		kMaxPower, // 最大の時の力
		kFloatEnd,
	};
	static const std::vector<std::string> fNames;
	static std::vector<float> fParas_;

	static std::unique_ptr<GlobalVariableUser> globalVariable_;

private:
	std::list<int> nums_;
	float power_;
	float firstRotate_;
	float lRotate_;
	float rRotate_;
	float time_;
	float preLRotate_;
	float preRRotate_;
	float maxRotate_;
};