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

StageScene::StageScene()
{
	FirstInit();

	WaterChunkChip::StaticInitialize();
	WaterChunk::StaticInitialize();
	GravityArea::StaticInitialize();
	Item::StaticInitialize();
	RequiredObject::StaticInitialize();
	WaterWave::StaticInitialize();
	BackGroundObject::StaticInitialize();
	WaterChunkQuadrangle::StaticInitialize();

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
	camera_->transform_.translate_.z = -50.0f;
	camera_->Update();

	WaterChunk::SetPlayer(player_.get());
	WaterChunkQuadrangle::SetPlayer(player_.get());

	goal_ = std::make_unique<Goal>();

	bg_ = std::make_unique<BackGround>();
	bg_->Update(camera_.get());

	bgm_.LoadMP3("Music/ingame.mp3", "StageBGM", bgmVolume_);
	seDead_.LoadWave("SE/gameOver.wav", "DEADSOUND", bgmVolume_);
	waterEffect_ = std::make_unique<WaterEffect>(camera_->transform_.translate_);

	optionUI_ = std::make_unique<OptionUI>();

	tensionUI_ = std::make_unique<TensionUI>();
}

void StageScene::Initialize()
{

	AudioManager::GetInstance()->AllStop();

	player_->Initialize();
	baby_->Initialize();
	camera_->transform_.translate_.x = player_->GetPosition().x;
	camera_->transform_.translate_.y = player_->GetPosition().y;
	camera_->Update();
	waterManager_->Initialize();
	itemManager_->Initialize();
	goal_->Initialize();

	effeGoalGuid_->Initialize(player_->GetPositionPtr(),&goal_->GetPosition(),camera_.get());
	std::list<QuotaSendData>datas = itemManager_->GetQuotaData();
	for (auto& data : datas) {
		effeGoalGuid_->SetQuota(data.pos, *data.size, data.isHit);
	}
	effeGoalGuid_->Update();

	enemyManager_->Initialize(player_.get());


	bgm_.Play(true);
	backGroundObjectManager_->Initialize();
	waterEffect_->Initialize();
	bg_->Initialize();

	isCanGoal_ = false;

	optionUI_->Initialize(OptionUI::kStage);
	isOptionOpen_ = false;

	tensionUI_->Initialize();
	// テンション関係
	tensionUI_->Update(0.0f, 0);
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
#endif // _DEBUG

	float deltaTime = frameInfo_->GetDeltaTime();
	
	//planetManager_->Update(deltaTime);

	player_->Update(deltaTime);
	baby_->Update(deltaTime);

	enemyManager_->Update(deltaTime, camera_.get(), baby_->GetFace());

	waterManager_->Update(deltaTime, camera_.get());

	itemManager_->Update(deltaTime, camera_.get());
	isCanGoal_ = itemManager_->GetIsCanGoal();

	backGroundObjectManager_->Update(deltaTime);

	if (isCanGoal_) {
		goal_->Update(deltaTime);
	}

	debugCamera_->Update();
	if (debugCamera_->IsDebug()) {
		debugCamera_->DebugUpdate();
	}
	else {
		// 今テキトーにカメラの位置変えてるけどfollowCameraなどの処理書くところ
		camera_->transform_.translate_.x = player_->GetPosition().x;
		camera_->transform_.translate_.y = player_->GetPosition().y;
		camera_->Update();
	}
	// テンション関係
	tensionUI_->Update(baby_->GetTension(), baby_->GetFace());

	// 背景更新
	bg_->Update(camera_.get());

	collisionManager_->CheckCollision();

	
	waterEffect_->Update(deltaTime);
	//if (isCanGoal_) {
		effeGoalGuid_->Update();
	//}

	if (isOptionOpen_) {
		isOptionOpen_ = optionUI_->Update();
	}
	else {
		SceneChange();
	}
}



void StageScene::Draw()
{
	instancingmodelManager_->Clear();
	particleManager_->Clear();

	// ポストエフェクトの用の描画
	MakePostEffect();

	Kyoko::Engine::PreDraw();
	
	waterEffect_->Draw();

	player_->Draw(camera_.get());
	baby_->Draw(camera_.get());
	waterEffect_->WaterAreaDraw();

	//waveFloor_->Draw();

	itemManager_->Draw();

	if (isCanGoal_) {
		goal_->Draw();
	}

	enemyManager_->Draw();
	
	//インスタンシング関係のすべてを描画
	instancingmodelManager_->Draw(*camera_.get());
	particleManager_->Draw(*camera_.get());

	//if (isCanGoal_) {
		effeGoalGuid_->Draw(camera_.get());
	//}

	player_->DrawUI();

	if (isOptionOpen_) {
		optionUI_->Draw();
	}
	tensionUI_->Draw();

	BlackDraw();

#ifdef _DEBUG
	AnimationManager::GetInstance()->Draw(camera_.get());
#endif // アニメーションエディター用描画

	// フレームの終了
	Kyoko::Engine::PostDraw();
}

void StageScene::SceneChange()
{
	if (input_->PressedKey(DIK_LSHIFT) && input_->PressedKey(DIK_SPACE)) {
		// シーン切り替え
		ChangeScene(CLEAR);
		bgm_.Stop();
		player_->Finalize();
	}
	if (goal_->IsClear()) {
		// シーン切り替え
		if (stageNo_ + 1 == maxStageNo_) {
			ChangeScene(SELECT);
		}
		else {
			stageNo_++;
			ChangeScene(STAGE);
		}
		bgm_.Stop();
		player_->Finalize();
	}
	if (player_->GetIsHitEnemy()) {
		ChangeScene(SELECT);
		bgm_.Stop();
		player_->Finalize();
		seDead_.Play();
	}

	//optionを開く
	if (input_->PressedGamePadButton(Input::GamePadButton::START) && !isOptionOpen_) {
		isOptionOpen_ = true;
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
}

