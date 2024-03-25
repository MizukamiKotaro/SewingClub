#pragma once
#include "GameElement/Charactor/Charactor.h"
#include "Vector2.h"
#include "Vector3.h"
#include "GameElement/Yarn/Yarn.h"
#include <list>

class Input;
class WaterManager;

class Player : public Charactor
{
public:

	Player();

	void Initialize() override;

	void Update(float deltaTime) override;

	void Draw(const Camera* camera) override;

public:
	const Vector3& GetPosition() const;

	//void SetIsInWater(bool is) { isInWater_ = is; }

private:
	void OnCollision(const Collider& collider) override;

	void SetCollider();

	void SetGlobalVariable() override;

	void ApplyGlobalVariable() override;

	void Move(float deltaTime);

	void PopUpFromWater();

	void ComeToWater();

	void OutWater(float deltaTime);

	void UpdateDelayProcess(float deltaTime);

	void Reset();

	void InitializeFloating();

	void UpdateFloating();

private:

	Input* input_ = nullptr;
	WaterManager* waterManager_ = nullptr;

	enum FloatParamater {
		kAcceleration, // 加速度
		kAttenuation, // 減衰率
		kMaxSpeed, // 最大速度
		kMinSpeed, // 最低速度
		kInterpolationRate, // 補間の割合
		kFloatingTime, // 上下挙動の1往復の時間
		kGravity, // 重力
		kOutWaterAcceleration, // 水から飛び出したときの加速度
		kOutWaterTime, // 水から飛び出したときに加速させる時間
		kMinPositionY, // プレイヤーの最低の高さ
		kGravityDown, // 降下中の重力
		kGravityWater, // 水の塊の重力
		kKeepSpeedTime, // 加速を維持する時間
		kWaterSize, // プレイヤーが生成する水のサイズ
		kWaterSizeMove, // ジャンプで生成する水のサイズ
		kDelayTime,
		kFloatEnd,
	};

	std::string fNames[kFloatEnd] = {
		"加速度",
		"減速率",
		"最大速度",
		"最低速度",
		"補間の割合",
		"上下挙動の1往復の時間",
		"重力加速度",
		"水から飛び出したときの加速度",
		"水から飛び出したときに加速させる時間",
		"プレイヤーの最低の高さ",
		"降下中の重力",
		"水の塊の重力",
		"加速を維持する時間",
		"プレイヤーが生成する水のサイズ",
		"ジャンプで生成する水のサイズ",
		"水の生成に遅延させる時間"

	};

	float fParas_[kFloatEnd];

	enum BoolParamater {
		kGravityArea,
		kAddWaterTriger,
		kAddWaterMove,
		kBoolEnd,
	};

	std::string bNames[kBoolEnd] = {
		"水ごとに重力がありか",
		"ボタンを押したときに水を生成するか",
		"ジャンプしたときに水を生成するか"
	};

	bool bParas_[kBoolEnd];

	Vector3 velocity_; // 速度
	Vector2 vector_; // 移動方向ベクトル
	float speed_;
	float addAcceleration_;
	Vector2 gravityVelocity_;
	Vector2 gravityPos_;
	bool isGravity_;

	float timeCount_;

	bool preIsInWater_;
	bool isInWater_;

	float floatingParameter_;

	std::unique_ptr<Yarn> yarn_;

	struct DelayProcess
	{
		Vector2 position_;
		float count_;
	};

	std::list<DelayProcess> delayProcess_;
	bool isMemoryPos_;
};