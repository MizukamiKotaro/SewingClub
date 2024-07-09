#pragma once
#include <string>
#include <memory>
#include "StageEditor/StageEditor.h"
#include "InstancingModelManager.h"
#include "CollisionSystem/Collider/Collider.h"
#include"Audio/Audio.h"
#include "Game/GameElement/Animation/Animation2D.h"
#include"GameElement/Effects/GoalDusts/GoalDusts.h"

class Camera;
class Wave;

class Goal : public Collider
{
public:
	Goal();

	void Initialize();

	bool Update(float deltaTime);

	void Draw() const;

	const bool IsClear() const { return isHit_; }

	const Vector3& GetPosition()const { return position_; }

	void SetGoal(const bool& flag) { isGoal_ = flag; }
private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

	void OnCollision(const Collider& collider) override;
	void SetCollider();

	void UpDownScale();

private:
	InstancingModelManager* instancingManager_;
	const InstancingMeshTexData* modelData_;

	std::unique_ptr<GlobalVariableUser> globalVariable_;
	std::unique_ptr<StageEditor> stageEditor_;
	std::unique_ptr<Animation2D> animation_;

private:
	static float deleteTime_;

	bool isHit_;

	Vector3 position_;
	float maxScale_;
	float scale_;
	float rotate_;
	bool isSmall_;
	float time_;
	Vector4 color_;
	bool isGoal_ = false; // ゴールできるか

	Audio seOpenGoal_;

	Audio seGoal_;
	bool isPlayed_ = false;

	std::unique_ptr<EffectGoalDusts>edusts_;
};