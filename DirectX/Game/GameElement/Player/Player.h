#pragma once
#include "GameElement/Charactor/Charactor.h"
#include "Vector2.h"
#include "Vector3.h"

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

private:
	enum FloatPara {
		Acceleration, // 加速度
		Attenuation, // 減衰率
		MaxSpeed, // 最大速度
		MinSpeed, // 最低速度
		InterpolationRate, // 補間の割合
		kFloatEnd,
	};

	std::string fNames[kFloatEnd] = {
		"加速度",
		"減速率",
		"最大速度",
		"最低速度",
		"補間の割合"
	};

	float fParas_[kFloatEnd];

	Vector3 velocity_; // 速度
	Vector2 vector_; // 移動方向ベクトル
	float speed_;

private:
	Input* input_ = nullptr;
	
};