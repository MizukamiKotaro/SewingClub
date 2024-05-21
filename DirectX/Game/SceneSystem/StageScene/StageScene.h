#pragma once

#include <memory>
#include <unordered_map>
#include "SceneSystem/IScene/IScene.h"

#include "GameElement/Player/Player.h"
#include "GameElement/WaterChunk/WaterChunk.h"
#include "GameElement/Goal/Goal.h"
#include "GameElement/WaterEffect/WaterEffect.h"
#include "GameElement/Baby/Baby.h"


class InstancingModelManager;
class CollisionManager;
class WaterManager;
class ItemManager;
class ParticleManager;
class EnemyManager;
class BackGroundObjectManager;

class StageScene : public IScene
{
public:

	StageScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

	void MakePostEffect();

private:

	std::unique_ptr<Player> player_;
	std::unique_ptr<Baby> baby_;
	std::unique_ptr<Goal> goal_;
	std::unique_ptr<WaterEffect> waterEffect_;

	float time_ = 0.0f;

	InstancingModelManager* instancingmodelManager_;
	CollisionManager* collisionManager_;
	WaterManager* waterManager_;
	ItemManager* itemManager_;
	ParticleManager* particleManager_;
	EnemyManager* enemyManager_;
	BackGroundObjectManager* backGroundObjectManager_;
};
