#pragma once

#include <memory>
#include <unordered_map>
#include "SceneSystem/IScene/IScene.h"

#include "GameElement/Player/Player.h"
#include "GameElement/WaveFloor/WaveFloor.h"
#include "InstancingModelManager.h"
#include "GameElement/Wave/Wave.h"
#include "GameElement/WaterChunk/WaterChunk.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "GameElement/Planet/PlanetManager.h"
#include "GameElement/Client/ClientManager.h"
#include "GameElement/Item/ItemManager.h"
#include "GameElement/Goal/Goal.h"
#include "GameElement/Animation/AnimationManager.h"

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
	std::unique_ptr<AnimationManager> testModel_;

	float time_ = 0.0f;

	InstancingModelManager* instancingmodelManager_;
	CollisionManager* collisionManager_;
	WaterManager* waterManager_;
	PlanetManager* planetManager_;
	ClientManager* clientManager_;
	ItemManager* itemManager_;
};
