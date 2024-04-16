#pragma once
#include <string>
#include <memory>
#include "StageEditor/StageEditor.h"
#include "InstancingModelManager.h"
#include "CollisionSystem/Collider/Collider.h"
#include"Audio/Audio.h"

class Camera;
class Wave;

class Goal : public Collider
{
public:
	Goal();

	void Initialize();

	void Update(float deltaTime);

	void Draw() const;

	const bool IsClear() const { return isHit_; }

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

	void OnCollision(const Collider& collider) override;
	void SetCollider();

private:
	InstancingModelManager* instancingManager_;
	const InstancingMeshTexData* modelData_;

	std::unique_ptr<GlobalVariableUser> globalVariable_;
	std::unique_ptr<StageEditor> stageEditor_;

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

	Audio seGoal_;
};