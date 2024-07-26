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
#include "GameElement/Camera/FollowCamera.h"
#include "GameElement/Camera/GoalCamera.h"
#include "GameElement/Camera/ZoomUpCamera.h"
#include"GameElement/GameOver/GameOver.h"
#include"GameElement/GameClear/GameClear.h"

#include "GameElement/PopupUI/PopupUI.h"
#include"GameElement/Effects/GetItem/GetItem.h"
#include "GameElement/Effects/ComboEffect.h"
#include"GameElement/InGameHUD/InGameHUD.h"
#include "GameElement/FragmentVignette/FragmentVignette.h"
#include "Audio/Audio.h"
#include"GameElement/SceneAcquisition/SceneAcquisition.h"

#ifdef _DEBUG
#include "GameElement/EditorSystem/EditorSystem.h"
#endif // _DEBUG


class InstancingModelManager;
class CollisionManager;
class WaterManager;
class ItemManager;
class ParticleManager;
class EnemyManager;
class BackGroundObjectManager;
class BabyTensionEffectManager;

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
	std::unique_ptr<FragmentVignette> fragmentVignette_;
	float time_ = 0.0f;

	InstancingModelManager* instancingmodelManager_;
	CollisionManager* collisionManager_;
	WaterManager* waterManager_;
	ItemManager* itemManager_;
	BabyTensionEffectManager* tensionEffectManager_;

	//
	ParticleManager* particleManager_;
	EnemyManager* enemyManager_;
	std::unique_ptr<UIGoalGuidance>effeGoalGuid_;

	Audio bgm_;
	Audio seDead_;
	float bgmVolume_ = 0.15f;

	Audio seClear_;
	Audio seOpenOption_;

#ifdef _DEBUG
	std::unique_ptr<EditorSystem> editorSystem_;
#endif // _DEBUG


	//ステージ最大数
	int maxStageNo_ = 8;

	bool isCanGoal_ = false; // ゴールできるか
	bool isGoalTransition_ = false; // ゴールできるようになったらの演出
	int countIndex = 0; // 初期化用の使ってるけどいらない子です

	// 背景
	std::unique_ptr<BackGround> bg_;
	BackGroundObjectManager* backGroundObjectManager_;

	//option関係
	std::unique_ptr<OptionUI>optionUI_;
	bool isOptionOpen_ = false;
	UpdateAnswer ans_;

	// テンション用UI
	std::unique_ptr<TensionUI> tensionUI_;

	// ゲーム開始時処理
	std::unique_ptr<PopupUI> popupUI_;
	bool isGameStarted_ = false;

	// カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<GoalCamera> goalCamera_;
	std::unique_ptr<ZoomUpCamera> zoomUpCamera_;
	float cameraOffset_ = -50.0f;

	enum playScenes {
		kPlay,
		kGameToClear,
		_countPlayScenes
	};

	playScenes nowScene = kPlay;

	//演出のシングルトン
	EffectGetItem* effeGetItem_=nullptr;

	// コンボエフェクト
	ComboEffectManager* comboEffect_ = nullptr;

	//HUDとか
	std::unique_ptr<InGameHUD>ingameHUD_;

	Audio se_babyNormal;

	//シーン全体の保存
	SceneAcquisition* sceneAcuition_;
};
