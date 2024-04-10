#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include <string>
#include <memory>
#include "InstancingModelManager.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include "RandomGenerator/RandomGenerator.h"

class Camera;

class DeadLine
{
public:

	DeadLine(Camera* camera, const Vector3* playerPos);

	void Initialize();

	void Update(const float& deltaTime);

	void Draw();

	bool IsPlayerDead() { return position_ > playerPos_->x; }

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

	void GenerateChips(const float& deltaTime);

private:
	InstancingModelManager* instancingManager_;
	const ModelData* modelData_;
	RandomGenerator* rand_;

	std::unique_ptr<GlobalVariableUser> globalVariable_;
private:

	struct  Chip
	{
		float countTime_;
		Vector3 position_;
		Vector3 scale_;
		bool isActive_;
	};

	const Vector3* playerPos_;

	Camera* camera_;

	float lifeTime_;
	float firstScale_;
	int screenDivision_;
	float generateTime_;
	float speed_;
	float firstPosition_;

	struct Ganerater
	{
		float generateTime;
		float timeCount;
	};
	std::vector<Ganerater> generator_;

	float position_;

	std::list<std::unique_ptr<Chip>> chips_;

};