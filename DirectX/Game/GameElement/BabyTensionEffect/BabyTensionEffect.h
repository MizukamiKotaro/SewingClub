#pragma once
#include <Vector2.h>
#include <Vector3.h>
#include "GlobalVariables/GlobalVariableUser.h"
#include <memory>
#include <vector>

class RandomGenerator;
class Camera;

class BabyTensionEffectChip
{
public:
	BabyTensionEffectChip(const float& tension, const float& angle, const float& length, const float& rotate, const float& speed, const Vector2& scale, const Vector2& gagePos, const float& generateTime, const float& floatTime);
	static void StaticInisialize();
	static void SetBabyPos(const Vector3* pos) { babyPos_ = pos; }

	const float GetTension();

private:
	Vector2 gagePos_;
	Vector3 localPos_;
	float angle_;
	float rotate_;
	float speed_;
	Vector3 pos_;
	float tensionNum_;
	float scale_;
	Vector2 maxScale_;
	float length_;
	float generateTime_;
	float time_;

	float floatTime_;
	bool isActive_;
private:
	static const Vector3* babyPos_;
};

class BabyTensionEffect 
{
public:
	BabyTensionEffect(const float& tensionNum);

	static void StaticInitialize();
	static void StaticUpdate();

	void Update(const float& deltaTime);
	void Draw(const Camera& camera);

private:
	static void SetGlobalVariable();
	static void ApplyGlobalVariable();
	static void InitializeGlobalVariable();

private:
	bool isActive_;
	std::vector<std::unique_ptr<BabyTensionEffectChip>> chips_;
	float time_;

private:
	static RandomGenerator* rand_;
	static std::unique_ptr<GlobalVariableUser> globalVariable_;

	enum FloatParamater {
		kGenerateTime, // 生成するまでの時間
		kGenerateRagTime, // 生成の時間差
		kGenerateEndTime, // 生成にかかる時間
		kStayTime, // 生成が終わってからの待機時間
		kMoveSpeed, // 移動スピード
		kMoveRagTime, // 移動のタイムラグ
		kGenerateLength, // 生成場所の距離
		kLengthWidth, // 距離の幅
		kFloatTime, // ふよふよの時間
		kScale, // スケール
		kFloatEnd,
	};
	static std::vector<const char*> fNames;
	static std::vector<float> fParas_;

	static Vector2 baseScale_;
	static Vector2 localGagePos_;
	static Vector2 localMinGagePos_;
	static Vector2 localMaxGagePos_;
	static int32_t maxChipNum_;
	static int32_t minChipNum_;
};