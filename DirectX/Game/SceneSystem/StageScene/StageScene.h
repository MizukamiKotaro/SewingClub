#pragma once

#include <memory>
#include <unordered_map>
#include "SceneSystem/IScene/IScene.h"

#include "GameElement/Player/Player.h"
#include "GameElement/WaveFloor/WaveFloor.h"
#include "GameElement/Wave/Wave.h"
#include "GameElement/WaterChunk/WaterChunk.h"
#include "GameElement/Goal/Goal.h"
#include "GameElement/DeadLine/DeadLine.h"

class InstancingModelManager;
class CollisionManager;
class WaterManager;
class PlanetManager;
class ClientManager;
class ItemManager;
class ParticleManager;
class EnemyManager;
#include"EffectOutWater.h"

class StageScene : public IScene
{
public:

	StageScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

	void WaveUpdate();

	//シーン変更
	void SceneChange();
private:

	std::unique_ptr<Player> player_;
	std::unique_ptr<WaveFloor> waveFloor_;
	std::list<std::unique_ptr<Wave>> waves_;
	std::unique_ptr<Goal> goal_;
	std::unique_ptr<DeadLine> deadLine_;

	float time_ = 0.0f;

	InstancingModelManager* instancingmodelManager_;
	CollisionManager* collisionManager_;
	WaterManager* waterManager_;
	PlanetManager* planetManager_;
	ClientManager* clientManager_;
	ItemManager* itemManager_;

	//エフェクト関連のデータ
	EffectOutWater* eOutWater_;
	ParticleManager* particleManager_;
	EnemyManager* enemyManager_;

	Audio bgm_;
	float bgmVolume_ = 0.5f;
};
