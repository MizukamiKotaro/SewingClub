#include "StageScene.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Kyoko.h"
#include "RandomGenerator/RandomGenerator.h"
#include "GameElement/WaterManager/WaterManager.h"
#include "InstancingModelManager.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "GameElement/Planet/PlanetManager.h"
#include "GameElement/Client/ClientManager.h"
#include "GameElement/Item/ItemManager.h"
#include "ParticleManager.h"
#include "GameElement/Enemy/EnemyManager.h"

StageScene::StageScene()
{
	FirstInit();

	Yarn::StaticInitialize();
	WaveFloorChip::StaticInitialize();
	WaveFloor::StaticInitialize();
	Wave::StaticInitialize();
	WaterChunk::StaticInitialize();
	GravityArea::StaticInitialize();
	Client::StaticInitialize();
	Planet::StaticInitialize();
	Item::StaticInitialize();

	instancingmodelManager_ = InstancingModelManager::GetInstance();
	collisionManager_ = CollisionManager::GetInstance();
	waterManager_ = WaterManager::GetInstance();
	planetManager_ = PlanetManager::GetInstance();
	clientManager_ = ClientManager::GetInstance();
	itemManager_ = ItemManager::GetInstance();
	particleManager_ = ParticleManager::GetInstance();
	enemyManager_ = EnemyManager::GetInstance();

	waterManager_->InitializeGlobalVariables();
	itemManager_->InitializeGlobalVariables();

	player_ = std::make_unique<Player>();
	camera_->transform_.translate_.z = -50.0f;
	camera_->Update();

	goal_ = std::make_unique<Goal>();

	planetManager_->SetPlayer(player_.get());

	waveFloor_ = std::make_unique<WaveFloor>();

	deadLine_ = std::make_unique<DeadLine>(camera_.get(),player_->GetPositionPtr());

	eOutWater_ = EffectOutWater::GetInstance();
	eOutWater_->SetUp();
}

void StageScene::Initialize()
{
	player_->Initialize();
	camera_->transform_.translate_.x = player_->GetPosition().x;
	camera_->transform_.translate_.y = player_->GetPosition().y;
	camera_->Update();
	waves_.clear();
	waterManager_->Initialize();
	//planetManager_->Initialize();
	clientManager_->Clear();
	itemManager_->Initialize();
	goal_->Initialize();

	eOutWater_->Initialize();
	deadLine_->Initialize();
	enemyManager_->Initialize();
}

void StageScene::Update()
{
	

	collisionManager_->Clear();

#ifdef _DEBUG
	Yarn::StaticUpdate();
	WaveFloorChip::StaticUpdate();
	WaveFloor::StaticUpdate();
	Wave::StaticUpdate();
	WaterChunk::StaticUpdate();
	GravityArea::StaticUpdate();
	Client::StaticUpdate();
	Planet::StaticUpdate();
	Item::StaticUpdate();

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

	WaveUpdate();

	float deltaTime = frameInfo_->GetDeltaTime();

	//planetManager_->Update(deltaTime);

	player_->Update(deltaTime);

	enemyManager_->Update(deltaTime, camera_.get());

	deadLine_->Update(deltaTime);

	clientManager_->Update(deltaTime);

	waterManager_->Update(deltaTime, camera_.get());

	itemManager_->Update(deltaTime, camera_.get());

	goal_->Update(deltaTime);

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

	waveFloor_->Update();

	collisionManager_->CheckCollision();

	eOutWater_->Update();

	SceneChange();
}

void StageScene::Draw()
{
	instancingmodelManager_->Clear();
	particleManager_->Clear();

	Kyoko::Engine::PreDraw();

	player_->Draw(camera_.get());

	//waveFloor_->Draw();

	waterManager_->Draw();

	itemManager_->Draw();

	goal_->Draw();

	enemyManager_->Draw();

	//planetManager_->Draw();

	clientManager_->Draw();

	deadLine_->Draw();


	eOutWater_->Draw();

	//インスタンシング関係のすべてを描画
	instancingmodelManager_->Draw(*camera_.get());
	particleManager_->Draw(*camera_.get());

	player_->DrawClient();


	BlackDraw();


	// フレームの終了
	Kyoko::Engine::PostDraw();
}

void StageScene::WaveUpdate()
{
	RandomGenerator* rand = RandomGenerator::GetInstance();

	time_ += frameInfo_->GetDeltaTime();
	if (time_ >= 5.0f) {
		Vector3 pos = rand->RandVector3(-10.0f, 10.0f);
		float power = rand->RandFloat(0.8f, 1.5f);
		float radius = rand->RandFloat(0.01f, 2.0f);
		waves_.push_back(std::make_unique<Wave>(pos, power, radius));
		pos = rand->RandVector3(-10.0f, 10.0f);
		power = rand->RandFloat(0.8f, 1.5f);
		radius = rand->RandFloat(0.01f, 2.0f);
		waves_.push_back(std::make_unique<Wave>(pos, power, radius));

		time_ = 0.0f;
	}

	for (std::list<std::unique_ptr<Wave>>::iterator it = waves_.begin(); it != waves_.end(); ) {
		(*it)->Update();

		waveFloor_->HitTest(*(*it).get());

		if ((*it)->IsFinish()) {
			(*it).reset();
			it = waves_.erase(it);
		}
		else {
			it++;
		}
	}
}

void StageScene::SceneChange()
{
	if (input_->PressedKey(DIK_LSHIFT) && input_->PressedKey(DIK_SPACE)) {
		// シーン切り替え
		ChangeScene(CLEAR);
	}
	if (goal_->IsClear()) {
		// シーン切り替え
		ChangeScene(SELECT);
	}
	if (deadLine_->IsPlayerDead() || player_->GetIsHitEnemy()) {
		ChangeScene(SELECT);
	}
}

