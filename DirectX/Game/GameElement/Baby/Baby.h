#pragma once

#include <string>
#include <memory>
#include "GlobalVariables/GlobalVariableUser.h"
#include "GameElement/Player/GravityAreaSearch.h"
#include "GameElement/Charactor/Charactor.h"
#include "GameElement/Animation/Animation2D.h"
#include "GameElement/Effects/EffectEnterWater.h"

class Player;

class Baby : public Charactor
{
public:
	Baby(Player* player);

	void Initialize() override;

	void Update(float deltaTime) override;

	void Draw(const Camera* camera) override;

	void EffectDraw();
	const int GetFace() const { return int(tension_.face); }
	const float& GetTension() const { return tension_.tension; }
	const bool GetIsCry() const;
	const bool GetIsSuperSuperSmile() const;

private:
	void OnCollision(const Collider& collider) override;
	void SetCollider();
	void SetGlobalVariable() override;

	void ApplyGlobalVariable() override;

	void OutWaterUpdate(const float& deltaTime);
	void InWaterUpdate(const float& deltaTime);

	void InitializeGlobalVariable();

	void YarnUpdate();
	void PulledUpdate(const Vector3& vect, const float& length);

	void TensionInitialize();
	void TensionUpdate(const float& deltaTime);
	void TensionFaceUpdate();

private:
	std::unique_ptr<Model> baby_;
	Player* player_;
	std::unique_ptr<GravityAreaSearch> gravityAreaSearch_;

	Vector3 prePosition_;

	bool preIsInWater_;
	bool isInWater_;
	bool isFollowWater_;

	bool isCircleWater_;

	Vector3 velocity_;
	float speed_;

	Vector2 waterPos_;
	Vector2 waterGravityPos_;
	float waterRadius_;

	Vector2 startPos_;
	Vector2 endPos_;
	float startScale_;
	float endScale_;
	float waterRotate_;

	enum Face {
		kNormal,
		kSmile,
		kSuperSmile,
		kAnxiety,
		kCry,
		kFaceEnd,
	};

	struct Tension
	{
		float tension = 0.0f;
		float flyTime = 0.0f;
		float playerInWaterTime = 0.0f;
		float cryTime = 0.0f;
		float inWaterTime = 0.0f;
		float superTime = 0.0f;
		Face face = kNormal;
	};
	Tension tension_;

	enum FloatParamater {
		kMaxPlayerLength, // 加速度が最大になるときのプレイヤーとの距離
		kLimitePlayerLength, // プレイヤーとの限界距離
		kMaxAcceleration, // 加速度の最大
		kMaxSpeed, // 最大速度
		kMinSpeed, // 最低速度
		kBuoyancy, // 水の浮力
		kGravityWater, // 重力加速度
		kMaxSlide, // 加速度が最大の時の水の移動角度
		kMaxSlideLength, // 加速度が最大の時の水の移動距離
		kEffectEnterWaterVelo,//エフェクト発生時の速度に対する乗算
		kEffectSpawwnInterval,//エフェクト発生の間隔
		kFlyTime, // 空中のテンションアップするまで時間
		kUpTensionToFly, // 空中のテンションアップの数値
		kDownTensionBePlayerInWater, // プレイヤーが水中にいる時にテンションダウンの数値
		kPlayerInWaterTime, // プレイヤーが水中にいる時にテンションが下がる間隔の時間
		kCryTime, // 泣き止むまでの時間
		kResetTensionFromCry, // 泣き止んだ時のテンション
		kUpTensionOutWater, // 水から出たときのテンションアップの数値
		kInWaterTime, // 水から出たときのテンションアップするまでの水中の時間
		kSuperSuperSmileTime, // テンションマックス維持の時間
		kResetTensionFromSuper, // テンションマックスが終了したときのテンション
		kFloatEnd,
	};

	std::vector<const char*> fNames;
	std::vector<float> fParas_;

	std::unique_ptr<Model> yarn_;
	// アニメーション
	std::unique_ptr<Animation2D> animation_;

	std::unique_ptr<EffectEnterWater>effeEnterW_;
	float spawnEffectVelo_=0.1f;
	float spawnWaitCount_ = 0;
	float maxSpawnWaitCount_ = 10;
	bool isSpawnEffect_ = false;
};