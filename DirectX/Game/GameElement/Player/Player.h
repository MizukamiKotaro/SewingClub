#pragma once
#include "GameElement/Charactor/Charactor.h"
#include "Vector2.h"
#include "Vector3.h"
#include "GameElement/Yarn/Yarn.h"

class Input;

class Player : public Charactor
{
public:

	Player();

	void Initialize() override;

	void Update() override;

	void Draw(const Camera* camera) override;

private:
	void SetGlobalVariable() override;

	void ApplyGlobalVariable() override;

	void Move();

	void InitializeFloating();

	void UpdateFloating();

private:

	Input* input_ = nullptr;

	enum FloatParamater {
		kAcceleration, // 加速度
		kAttenuation, // 減衰率
		kMaxSpeed, // 最大速度
		kMinSpeed, // 最低速度
		kInterpolationRate, // 補間の割合
		kFloatingTime, // 上下挙動の1往復の時間
		kFloatEnd,
	};

	std::string fNames[kFloatEnd] = {
		"加速度",
		"減速率",
		"最大速度",
		"最低速度",
		"補間の割合",
		"上下挙動の1往復の時間"
	};

	float fParas_[kFloatEnd];

	Vector3 velocity_; // 速度
	Vector2 vector_; // 移動方向ベクトル
	float speed_;

	float floatingParameter_;

	std::unique_ptr<Yarn> yarn_;

};