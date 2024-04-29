#pragma once
#include "IEnemy.h"

class FollowEnemy : public IEnemy {
public:
	FollowEnemy(const int& no);

	static void StaticInitialize();
	static void SetPlayerPtr(const Vector3* pplayer);

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
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="isFollowing">true:player追従/false:初期位置に戻る</param>
	void Move(bool isFollowing);

private:
	static const ParticleMeshTexData* modelData_;
	static const Vector3* player_ptr;
	float kMaxDistance = 0.0f; // のちにstaticにしたい
	float speed_ = 0.0f; // 追従速度
	Vector3 initialPosition_; // 初期位置、positionは現在位置

	// パターン用フラグ
	bool isAreaMove_ = false; // 初期位置から一定範囲で追従
	bool isAutoMove_ = true; // 常に近ければ追従
	bool isReturn_ = true; // 初期位置に戻るか


};