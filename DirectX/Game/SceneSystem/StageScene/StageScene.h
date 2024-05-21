#pragma once

#include <memory>
#include <unordered_map>
#include "SceneSystem/IScene/IScene.h"

#include "GameElement/Player/Player.h"
#include "GameElement/WaterChunk/WaterChunk.h"
#include "GameElement/Goal/Goal.h"
#include "GameElement/WaterEffect/WaterEffect.h"
#include "GameElement/BackGround/BackGround.h"
#include"GameElement/Effects/EffectGoalGuidance.h"
#include "GameElement/Baby/Baby.h"
#include"GameElement/OptionUI/OptionUI.h"
#include "GameElement/TensionUI/TensionUI.h"

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

	void SceneChange();
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

	//
	ParticleManager* particleManager_;
	EnemyManager* enemyManager_;
	std::unique_ptr<UIGoalGuidance>effeGoalGuid_;

	Audio bgm_;
	Audio seDead_;
	float bgmVolume_ = 0.15f;

	//ステージ最大数
	int maxStageNo_ = 3;

	bool isCanGoal_ = false; // ゴールできるか

	// 背景
	std::unique_ptr<BackGround> bg_;
	BackGroundObjectManager* backGroundObjectManager_;

	//option関係
	std::unique_ptr<OptionUI>optionUI_;
	bool isOptionOpen_ = false;

	// テンション用UI
	std::unique_ptr<TensionUI> tensionUI_;

};
