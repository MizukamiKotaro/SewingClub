#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include <string>
#include <memory>
#include <unordered_map>
#include "InstancingModelManager.h"
#include "GlobalVariables/GlobalVariableUser.h"

class Camera;
class FrameInfo;

class WaterChunkChip {
public:

	WaterChunkChip(const Vector3& center, const Vector3& position, const float& rotate);

	static void StaticInitialize();

	void Initialize();

	void Update();

	void Draw() const;

	static void StaticUpdate();

	static const float GetScale() { return fParas_[kScale]; }

	void AddOutPower(float power);

	const Vector3 GetPosition() const { return position_; }

private:
	static void SetGlobalVariable();

	static void ApplyGlobalVariable();

private:
	enum FloatParamater {
		kAttenuation, // 減衰率
		kMinSpeed, // 最低速度
		kScale, // スケール
		kLoopTime, // 波の1往復の時間
		kFloatEnd,
	};
	static const std::string fNames[kFloatEnd];
	static float fParas_[kFloatEnd];

	static InstancingModelManager* instancingManager_;
	static const InstancingMeshTexData* modelData_;
	static FrameInfo* frameInfo_;

	static Vector3 scale_;

	static std::unique_ptr<GlobalVariableUser> globalVariable_;

private:
	struct WavePower
	{
		float radius;
		float time;
	};
	std::list<WavePower> wavePowers_;

	Vector3 center_;

	Vector3 velocity_;

	Vector3 position_;
	Vector3 rotate_;

	Vector3 ganeratePosition_;
};