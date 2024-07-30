#include "StageScene.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Kyoko.h"
#include "RandomGenerator/RandomGenerator.h"
#include "GameElement/WaterManager/WaterManager.h"
#include "InstancingModelManager.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "GameElement/Item/ItemManager.h"
#include "ParticleManager.h"
#include "GameElement/Enemy/EnemyManager.h"
#include "GameElement/WaterChunk/WaterWave.h"
#include "GameElement/BackGroundObject/BackGroundObjectManager.h"

#include "GameElement/Animation/AnimationManager.h"
#include"Audio/AudioManager/AudioManager.h"
#include "GameElement/BabyTensionEffect/BabyTensionEffectManager.h"
#include "GameElement/HitStop/HitStop.h"
#include"Ease/Ease.h"

StageScene::StageScene()
{
	FirstInit();

	WaterChunkChip::StaticInitialize();
	WaterChunk::StaticInitialize();
	GravityArea::StaticInitialize();
	ItemManager::StaticInitialize();
	WaterWave::StaticInitialize();
	BackGroundObject::StaticInitialize();
	WaterChunkQuadrangle::StaticInitialize();
	HitStop::Initialize();

	instancingmodelManager_ = InstancingModelManager::GetInstance();
	collisionManager_ = CollisionManager::GetInstance();
	waterManager_ = WaterManager::GetInstance();
	itemManager_ = ItemManager::GetInstance();
	particleManager_ = ParticleManager::GetInstance();
	enemyManager_ = EnemyManager::GetInstance();
	effeGoalGuid_ = std::make_unique<UIGoalGuidance>();
	backGroundObjectManager_ = BackGroundObjectManager::GetInstance();

	waterManager_->InitializeGlobalVariables();
	itemManager_->InitializeGlobalVariables();
	backGroundObjectManager_->InitializeGlobalVariables();

	player_ = std::make_unique<Player>();
	baby_ = std::make_unique<Baby>(player_.get());
	cameraOffset_ = -45.0f;
	camera_->transform_.translate_.z = cameraOffset_;
	camera_->Update();

	WaterChunk::SetPlayer(player_.get());
	WaterChunkQuadrangle::SetPlayer(player_.get());

	goal_ = std::make_unique<Goal>();

	bg_ = std::make_unique<BackGround>();
	bg_->Update(camera_.get());

	bgm_.LoadMP3("Music/ingame.mp3", "StageBGM", bgmVolume_);
	seDead_.LoadWave("SE/gameOver.wav", "DEADSOUND", bgmVolume_);

	seClear_.LoadMP3("SE/Scene/ingame_clear.mp3");
	seOpenOption_.LoadMP3("SE/Scene/autgame_poseOpen.mp3");

	waterEffect_ = std::make_unique<WaterEffect>(camera_->transform_.translate_);

	optionUI_ = std::make_unique<OptionUI>(OptionUI::kStage);

	tensionUI_ = std::make_unique<TensionUI>();

	popupUI_ = std::make_unique<PopupUI>();

	followCamera_ = std::make_unique<FollowCamera>();
	goalCamera_ = std::make_unique<GoalCamera>();
	zoomUpCamera_ = std::make_unique<ZoomUpCamera>();


	effeGetItem_ = EffectGetItem::GetInstance();
	effeGetItem_->ModelLoad();

	tensionEffectManager_ = BabyTensionEffectManager::GetInstance();
	tensionEffectManager_->FirstInitialize(baby_->GetPosPtr(), &camera_->transform_.translate_);

	comboEffect_ = ComboEffectManager::GetInstance();
	fragmentVignette_ = std::make_unique<FragmentVignette>();
	ingameHUD_ = std::make_unique<InGameHUD>();

	se_babyNormal.LoadMP3("SE/baby/baby_.mp3", "baby_normal");

#ifdef _DEBUG
	editorSystem_ = std::make_unique<EditorSystem>(camera_.get());
#endif // _DEBUG

	black_->size_ = { 1280.0f,720.0f };
	black_->pos_ = { 640.0f,360.0f };
	black_->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });
	black_->Update();
	sceneAcuition_ = SceneAcquisition::GetInstance();
	sceneTransition_ = std::make_unique<SceneTransitionEffect>("stage");
}

void StageScene::Initialize()
{

	AudioManager::GetInstance()->AllStop();
	HitStop::Initialize();
	player_->Initialize();
	baby_->Initialize();
	camera_->transform_.translate_.x = player_->GetPosition().x;
	camera_->transform_.translate_.y = player_->GetPosition().y;
	camera_->Update();
	waterManager_->Initialize();
	itemManager_->Initialize();
	goal_->Initialize();
	fragmentVignette_->Initialize();
	enemyManager_->Initialize(player_.get(),camera_.get());


	bgm_.Play(true);
	backGroundObjectManager_->Initialize();
	waterEffect_->Initialize();
	bg_->Initialize();

	isCanGoal_ = false;

	optionUI_->Initialize();
	isOptionOpen_ = false;

	// テンション関係
	tensionUI_->Initialize(baby_->GetTension(), baby_->GetFace());

	popupUI_->Initialize();
	isGameStarted_ = false;
	
	followCamera_->Initialize(player_->GetPositionPtr(), waterManager_->GetLimit().upperLimit, waterManager_->GetLimit().lowerLimit, 25.0f);
	camera_->transform_.translate_.z = followCamera_->Update().z + cameraOffset_;
	camera_->Update();


	countIndex = 0;
	isGoalTransition_ = false;

	effeGetItem_->Initialize();

	effeGoalGuid_->Initialize(player_->GetPositionPtr(), &goal_->GetPosition(), camera_.get());
	std::list<QuotaSendData>datas = itemManager_->GetQuotaData();
	for (auto& data : datas) {
		effeGoalGuid_->SetQuota(data.pos, *data.size, data.isHit);
	}
	effeGoalGuid_->Update();

	nowScene = playScenes::kPlay;

	tensionEffectManager_->Initialize();

	ingameHUD_->Initialize();

	goalCamera_->Initialize();

	sceneAcuition_->Initialize();
	sceneTransition_->Initialize(1.0f);
	isBlackOut_ = false;
	isDossolve_ = false;
	transitionRequest_ = Transition::kFromBlack;

	//シーンの初期状態
	nowScene = kPlay;
}

void StageScene::Update()
{

	collisionManager_->Clear();

#ifdef _DEBUG
	WaterChunkChip::StaticUpdate();
	WaterChunk::StaticUpdate();
	GravityArea::StaticUpdate();
	Item::StaticUpdate();
	WaterWave::StaticUpdate();

	AnimationManager::GetInstance()->Update();

	if (!ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
	}
	else {
		ImGui::DragFloat3("ポジション", &camera_->transform_.translate_.x, 0.01f);
		ImGui::DragFloat3("角度", &camera_->transform_.rotate_.x, 0.01f);
		camera_->Update();
		ImGui::End();
	}

	int num = stageNo_;
	ImGui::Begin("ステージ");
	ImGui::DragInt("現在のステージ", &stageNo_, 1, 0, 30);
	ImGui::End();

	if (input_->PressedKey(DIK_R) || num != stageNo_) {
		Initialize();
	}

	if (input_->PressedKey(DIK_SPACE)) {
		effeGetItem_->Spawn(player_->GetPosition());
	}
	editorSystem_->Update();
#endif // _DEBUG


	ans_ = UpdateAnswer();

	float deltaTime = frameInfo_->GetDeltaTime();

	HitStop::Update(deltaTime);

	switch (nowScene)
	{
	case StageScene::kPlay:
		if (!isOptionOpen_) {

			popupUI_->Update(deltaTime);
			if (!isGameStarted_) {
				if (popupUI_->GetPhase() == 1u) {
					// UIが出きったらスタート
					isGameStarted_ = true;
					followCamera_->SetFirstOffsetZ(0.0f);
				}
			}

			// ゴール遷移演出じゃなければ
			if (!isGoalTransition_ && isGameStarted_) {
				player_->Update(deltaTime);
				baby_->Update(deltaTime);
				fragmentVignette_->Update(baby_->GetFragmentHP());
				tensionEffectManager_->Update(deltaTime);
				enemyManager_->Update(deltaTime, camera_.get(), baby_->GetFace());
			}
			itemManager_->Update(deltaTime, camera_.get());

			waterManager_->CreateGimmickWater(itemManager_->GetWaterList());

			waterManager_->Update(deltaTime, camera_.get());

			isCanGoal_ = itemManager_->GetIsCanGoal();

			backGroundObjectManager_->Update(deltaTime);

			bool goal = false;
			goal = goal_->Update(deltaTime);

			// カメラ更新処理
			debugCamera_->Update();
			if (debugCamera_->IsDebug()) {
				debugCamera_->DebugUpdate();
			}
			else {
				Vector3 camera{};
				if (isCanGoal_ && countIndex == 0) {
					isGoalTransition_ = true;
					countIndex = 1;
					// ばぶー
					se_babyNormal.Play();
				}

				// 通常カメラ
				if (!isGoalTransition_) {
					camera = followCamera_->Update();
					if (isGameStarted_) {
						//camera.z = 0.0f;
					}
				}
				// 遷移カメラ
				else {
					camera = goalCamera_->Update(player_->GetPosition(), goal_->GetPosition(), deltaTime);
					// 待機中になったら
					if (goalCamera_->GetType() == 1u) {
						countIndex = 2;
						goal_->SetGoal(true);
						if (goal) {
							goalCamera_->SetNext();
						}
					}
					// おわったら初期化
					if (goalCamera_->GetFinishd()) {
						isGoalTransition_ = false;
						countIndex++;
						followCamera_->Reset();
					}
					//camera.z = 0.0f;
				}

				// 今テキトーにカメラの位置変えてるけどfollowCameraなどの処理書くところ
				camera_->transform_.translate_.x = camera.x;
				camera_->transform_.translate_.y = camera.y;
				camera_->transform_.translate_.z = camera.z + cameraOffset_;
				camera_->Update();
			}
			// テンション関係
			tensionUI_->Update(baby_->GetTension(), baby_->GetFace());

			// 背景更新
			bg_->Update(camera_.get());

			collisionManager_->CheckCollision();

			//水のうねうね
			waterEffect_->Update(deltaTime);

			comboEffect_->Update(deltaTime);

			effeGoalGuid_->Update();
		}
		else {
			ans_ = optionUI_->Update();
			if (ans_.audioOption){
				bgm_.Update();
				player_->SoundUpdate();
			}
		}

		break;

	case StageScene::kGameToClear:
		player_->ClearUpdate(deltaTime);
		baby_->ClearUpdate(deltaTime);
		
		// クリア時のカメラ遷移
		camera_->transform_.translate_ = zoomUpCamera_->Update(player_->GetClearTime(), deltaTime);
		camera_->Update();

		break;
	case StageScene::_countPlayScenes:
		break;
	default:
		break;
	}


	SceneChange();

	effeGetItem_->Update();
	ingameHUD_->Update();
}



void StageScene::Draw()
{
	instancingmodelManager_->Clear();
	particleManager_->Clear();

	// ポストエフェクトの用の描画
	MakePostEffect();

	Kyoko::Engine::PreDraw();

	//他シーンで映すすべてを描画
	sceneAcuition_->DrawScene();

	//option描画
	if (isOptionOpen_) {
		optionUI_->Draw();
	}

	BlackDraw();

	sceneTransition_->Draw();

#ifdef _DEBUG
	AnimationManager::GetInstance()->Draw(camera_.get());
#endif // アニメーションエディター用描画
	// フレームの終了
	Kyoko::Engine::PostDraw();
}

void StageScene::FromBlackInitialize()
{
	if (sameScene_) {
		sameScene_ = false;
		Initialize();
	}

	transitionTimeCount_ = 0.0f;
	black_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void StageScene::FromBlackUpdate()
{
	float delta = FrameInfo::GetInstance()->GetDeltaTime();
	if (delta >= 0.3f) {
		return;
	}
	transitionTimeCount_ += delta;

	float alpha =
		Ease::UseEase(1.0f, 0.0f, transitionTimeCount_, kTransitionTime, Ease::EaseInSine, 2);
	black_->SetColor({ 0.0f, 0.0f, 0.0f, alpha });

	if (transitionTimeCount_ >= kTransitionTime) {
		black_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
		transitionRequest_ = Transition::kOperation;
	}
}

void StageScene::ToBlackInitialize()
{
	transitionTimeCount_ = 0.0f;
	black_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
	sceneTransition_->Initialize(1.0f);
}

void StageScene::ToBlackUpdate()
{
	if (isDossolve_) {
		float deltaTime = frameInfo_->GetDeltaTime();
		if (sceneTransition_->PostSceneTransition(deltaTime)) {
			transitionRequest_ = Transition::kFromBlack;

			if (sceneNo_ != nextScene_) {
				sceneNo_ = nextScene_;
			}

		}
	}
	else {

		if (isBlackOut_) {
			transitionTimeCount_ = 10.0f;
		}

		transitionTimeCount_ += FrameInfo::GetInstance()->GetDeltaTime();

		float alpha =
			Ease::UseEase(0.0f, 1.0f, transitionTimeCount_, kTransitionTime, Ease::EaseInSine, 2);
		black_->SetColor({ 0.0f, 0.0f, 0.0f, alpha });

		if (transitionTimeCount_ >= kTransitionTime) {
			transitionRequest_ = Transition::kFromBlack;
			if (isBlackOut_) {
				black_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
			}
			else {
				black_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
			}
			if (sceneNo_ == nextScene_) {
				sameScene_ = true;
			}
			else {
				sceneNo_ = nextScene_;
			}
		}
	}
}

void StageScene::SceneChange()
{
	
		switch (nowScene)
		{
		case StageScene::kPlay:
			//optionが開かれていないとき
			if (!isOptionOpen_) {
				if (input_->PressedKey(DIK_LSHIFT) && input_->PressedKey(DIK_SPACE)) {
					// シーン切り替え
					//ChangeScene(CLEAR);
					bgm_.Stop();
					player_->Finalize();
				}

				if (goal_->IsClear()) {
					nowScene = kGameToClear;
					GameClear::tension_ = baby_->GetTension();
					GameClear::faceIndex_ = baby_->GetFace();
					//bgm_.Stop();
					player_->Finalize();
					seClear_.Play();
					isBlackOut_ = true;
					zoomUpCamera_->Initialize(camera_->transform_.GetWorldPosition(), goal_->GetPosition());
				}

				//ヒットによる処理
				/*if (player_->GetIsHitEnemy()) {
					nowScene = kGameOver;
					seDead_.Play();
				}*/
				//テンションのHPがなくなったときの処理
				if (baby_->GetIsGameOver()) {
					//nowScene = kGameOver;
					ChangeScene(GAMEOVER);
					//bgm_.Stop();
					player_->Finalize();
					seDead_.Play();
					isBlackOut_ = true;
				}

				//optionを開く
				if (input_->PressedGamePadButton(Input::GamePadButton::START) && !isOptionOpen_) {
					isOptionOpen_ = true;
					seOpenOption_.Play();
				}
			}
			else {
				if (ans_.backOption) {
					isOptionOpen_ = false;
				}
				else if (ans_.backSelect) {
					ChangeScene(SELECT);
					bgm_.Stop();
					player_->Finalize();
					isDossolve_ = true;
				}
				else if (ans_.backtitle) {
					ChangeScene(TITLE);
					bgm_.Stop();
					player_->Finalize();
					isDossolve_ = true;
				}
			}
			break;
		
		case StageScene::kGameToClear:
			if (player_->GetIsClear() && baby_->GetIsClear()) {
				//nowScene = kGameClear;
				ChangeScene(CLEAR);
				//bgm_.Stop();
				player_->Finalize();
			}
			break;
		case StageScene::_countPlayScenes:
			break;
		default:
			break;
		}	
	
}

void StageScene::MakePostEffect()
{
	waterEffect_->PreDrawBackGround();
	// 背景の描画
	bg_->Draw();
	backGroundObjectManager_->Draw();
	instancingmodelManager_->Draw(*camera_.get());
	particleManager_->Draw(*camera_.get());

	waterEffect_->PostDrawBackGround();

	instancingmodelManager_->Clear();
	particleManager_->Clear();

	waterEffect_->PreDrawWaterArea();
	// 水のエリアの描画
	waterManager_->Draw(camera_.get());

	player_->EffectDraw();

	baby_->EffectDraw();

	instancingmodelManager_->Draw(*camera_.get());
	particleManager_->Draw(*camera_.get());
	waterEffect_->PostDrawWaterArea();

	instancingmodelManager_->Clear();
	particleManager_->Clear();

	sceneAcuition_->PreDraw();

	waterEffect_->Draw();

	player_->Draw(camera_.get());
	baby_->Draw(camera_.get());
	waterEffect_->WaterAreaDraw();

	//waveFloor_->Draw();
	tensionEffectManager_->Draw();
	itemManager_->Draw();

	goal_->Draw();

	enemyManager_->Draw();

	effeGetItem_->Draw();

	comboEffect_->Draw();
	//インスタンシング関係のすべてを描画
	instancingmodelManager_->Draw(*camera_.get());
	particleManager_->Draw(*camera_.get());


	///いかUI
	fragmentVignette_->Draw();
	effeGoalGuid_->Draw(camera_.get());

	player_->DrawUI();

	tensionUI_->Draw();

	popupUI_->Draw();

	ingameHUD_->Draw();


	sceneAcuition_->PostDraw();

	sceneTransition_->DrawPE();

}

