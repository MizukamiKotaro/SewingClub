#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include <string>
#include <memory>
#include "GlobalVariables/GlobalVariableUser.h"

class Camera;
class FrameInfo;

class Wave {
public:
	Wave(const Vector3& position, float power, float radius);

	static void StaticInitialize();

	void Update();

	static void StaticUpdate();

	bool IsFinish() const;

private:
	static void SetGlobalVariable();

	static void ApplyGlobalVariable();

private:
	enum FloatParamater {
		kConvertingMagnification, // パワーから最大半径への変換倍率
		kExpandTime, // 広がりきる時間
		kFloatEnd,
	};
	static const std::string fNames[kFloatEnd];
	static float fParas_[kFloatEnd];

	static std::unique_ptr<GlobalVariableUser> globalVariable_;
	static FrameInfo* frameInfo_;

public:
	Vector3 position_;
	float power_;
	float maxRadius_;
	float radius_;
	float preRadius_;
	float time_;
	float firstRadius_;
};