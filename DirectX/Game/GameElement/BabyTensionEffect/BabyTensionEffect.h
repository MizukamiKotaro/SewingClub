#pragma once
#include <Vector2.h>
#include <Vector3.h>
#include "GlobalVariables/GlobalVariableUser.h"
#include <memory>
#include <vector>
#include "ParticleManager.h"

class RandomGenerator;

class BabyTensionEffectChip
{
public:
	BabyTensionEffectChip(const float& tension, const float& angle, const float& length, const float& rotate, const float& speed, const Vector2& scale, const Vector2& gagePos, const float& generateTime, const float& floatAngle, const float& floatTime, const float& floatLength);
	static void SetBabyPos(const Vector3* pos) { babyPos_ = pos; }
	static void StaticInitialize();
	const float GetTension();

	void GenerateUpdate(const float& deltaTime);
	void Update(const float& deltaTime);
	void FloatUpdate(const float& deltaTime);

	void Draw() const;

	const bool& GetIsActive() const { return isActive_; }

private:
	Vector2 gagePos_;
	Vector2 localPos_;
	Vector2 generatePos_;
	float rotate_;
	float speed_;
	Vector2 pos_;
	float tensionNum_;
	Vector2 scale_;
	Vector2 maxScale_;
	float generateTime_;
	float time_;
	float floatAngle_;
	float floatlength_;
	float floatTime_;
	float addFloatAngle_;
	bool isActive_;
	bool isMove_;
private:
	static const Vector3* babyPos_;
	static ParticleManager* instancingManager_;
	static const ParticleMeshTexData* modelData_;
};

class BabyTensionEffect 
{
public:
	BabyTensionEffect(const float& tensionNum);

	static void StaticInitialize();
	static void StaticUpdate();

	void Update(const float& deltaTime);
	void Draw() const;
	const float GetTension();
	const bool& GetIsActive() const { return isActive_; }
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
		kFloatLength, // ふよふよの幅
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