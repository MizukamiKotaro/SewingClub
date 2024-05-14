#pragma once

#include <string>
#include <memory>
#include "GlobalVariables/GlobalVariableUser.h"
//#include "GameElement/Player/GravityAreaSearch.h"
#include "GameElement/Charactor/Charactor.h"

class Player;

class Baby : public Charactor
{
public:
	Baby(Player* player);

	void Initialize() override;

	void Update(float deltaTime) override;

	void Draw(const Camera* camera) override;

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

private:
	Player* player_;
	//std::unique_ptr<GravityAreaSearch> gravityAreaSearch_;

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

	enum FloatParamater {
		kMaxPlayerLength, // 加速度が最大になるときのプレイヤーとの距離
		kLimitePlayerLength, // プレイヤーとの限界距離
		kMaxAcceleration, // 加速度の最大
		kMaxSpeed, // 最大速度
		kMinSpeed, // 最低速度
		kBuoyancy, // 水の浮力
		kMaxSlide, // 加速度が最大の時の水の移動角度
		kMaxSlideLength, // 加速度が最大の時の水の移動距離
		kFloatEnd,
	};

	std::vector<const char*> fNames;
	std::vector<float> fParas_;

	std::unique_ptr<Model> yarn_;
};