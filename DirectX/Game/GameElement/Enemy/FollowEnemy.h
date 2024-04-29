#pragma once
#include "IEnemy.h"

class FollowEnemy : public IEnemy {
public:
	FollowEnemy(const int& no);

	static void StaticInitialize(const Vector3* pplayer);

	void Initialize() override;

	void Update(const float& deltaTime, Camera* camera) override;

	void Draw() const override;

private:
	void OnCollision(const Collider& collider) override;

	void SetGlobalVariable() override;

	void ApplyGlobalVariable() override;

	/// <summary>
	/// 感知距離内かチェックする
	/// </summary>
	/// <returns>true:感知距離内/false:外</returns>
	bool ChackDistance() const;
	// 移動処理
	void Move();

private:
	static const ParticleMeshTexData* modelData_;
	static const Vector3* player_ptr;
	float kMaxDistance = 0.0f; // のちにstaticにしたい

};