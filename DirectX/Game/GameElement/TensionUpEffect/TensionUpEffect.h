#pragma once
#include <Vector2.h>
#include <Vector3.h>
#include "GlobalVariables/GlobalVariableUser.h"
#include <memory>
#include <vector>
#include "ParticleManager.h"

class RandomGenerator;

class TensionUpEffectChip
{
public:
	TensionUpEffectChip(const Vector2& pos, const float& speed, const float& time, const float& angle, const float& rotate, const Vector2& scale, const bool& isBaby);
	static void SetBabyPos(const Vector3* pos) { babyPos_ = pos; }
	static void SetCameraPos(const Vector3* pos) { cameraPos_ = pos; }
	static void StaticInitialize();

	void Update(const float& deltaTime);

	void Draw() const;

	const bool& GetIsActive() const { return isActive_; }

private:
	Vector2 velocity_;
	Vector2 pos_;
	Vector2 scale_;
	float rotate_;
	float time_;
	float finishTime_;
	bool isActive_;
	bool isBaby_;
	float a_;
private:
	static const Vector3* babyPos_;
	static const Vector3* cameraPos_;
	static ParticleManager* instancingManager_;
	static const ParticleMeshTexData* modelData_;
};

class TensionUpEffect
{
public:
	TensionUpEffect(const float& tensionNum, const bool& isBaby = true, const Vector2& pos = {});

	static void StaticInitialize();
	static void StaticUpdate();

	void Update(const float& deltaTime);
	void Draw() const;
	const bool& GetIsActive() const { return isActive_; }
private:
	static void SetGlobalVariable();
	static void ApplyGlobalVariable();
	static void InitializeGlobalVariable();

private:
	bool isActive_;
	std::vector<std::unique_ptr<TensionUpEffectChip>> chips_;
	float time_;

private:
	static RandomGenerator* rand_;
	static std::unique_ptr<GlobalVariableUser> globalVariable_;

	enum FloatParamater {
		kGenerateTime, // 生成するまでの時間
		kMoveSpeed, // 移動スピード
		kEndTime, // 消える時間
		kFloatEnd,
	};
	static std::vector<const char*> fNames;
	static std::vector<float> fParas_;

	static Vector2 baseScale_;
	static int32_t maxChipNum_;
	static int32_t minChipNum_;
};