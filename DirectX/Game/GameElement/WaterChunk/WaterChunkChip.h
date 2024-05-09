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

class WaterChunkChip {
public:

	WaterChunkChip(const Vector3& center, const Vector3& position, const float& rotate, const float& scale, const bool& isCircle = true);

	static void StaticInitialize();

	void Initialize();

	void Update(const float& deltaTime);

	void Draw() const;

	static void StaticUpdate();

	static const float GetScale() { return fParas_[kScale]; }

	void AddOutPower(const float& power, const bool& isDown);

	const Vector3& GetPosition() const { return position_; }
	const Vector3& GetCenter() const { return center_; }

	bool IsWave() const { return wavePowers_.size() > 0; }

	const float& GetRotate() const { return rotate_.z; }

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
	static Vector3 scale_;

	static std::unique_ptr<GlobalVariableUser> globalVariable_;

private:
	struct WavePower
	{
		float radius;
		float time;
		bool isDown;
	};
	std::list<WavePower> wavePowers_;

	Vector3 center_;

	Vector3 velocity_;

	Vector3 position_;
	Vector3 rotate_;
	Vector3 myScale_;

	Vector3 ganeratePosition_;
	bool isCircle_;
};