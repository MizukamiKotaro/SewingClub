#include "StageScene.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Kyoko.h"
#include "RandomGenerator/RandomGenerator.h"

StageScene::StageScene()
{
	FirstInit();

	Yarn::StaticInitialize();
	WaveFloorChip::StaticInitialize();
	WaveFloor::StaticInitialize();
	Wave::StaticInitialize();
	WaterChunk::StaticInitialize();

	instancingmodelManager_ = InstancingModelManager::GetInstance();
	collisionManager_ = CollisionManager::GetInstance();

	player_ = std::make_unique<Player>();

	waveFloor_ = std::make_unique<WaveFloor>();

	for (int i = 0; i < 3; i++) {
		water_[i] = std::make_unique<WaterChunk>();
	}
}

void StageScene::Initialize()
{
	player_->Initialize();
	waves_.clear();
}

void StageScene::Update()
{
	if (input_->PressedKey(DIK_RETURN)) {
		// シーン切り替え
		ChangeScene(CLEAR);
	}

	collisionManager_->Clear();

#ifdef _DEBUG
	Yarn::StaticUpdate();
	WaveFloorChip::StaticUpdate();
	WaveFloor::StaticUpdate();
	Wave::StaticUpdate();
	WaterChunk::StaticUpdate();

	ImGui::Begin("Camera");
	ImGui::DragFloat3("ポジション", &camera_->transform_.translate_.x, 0.01f);
	ImGui::DragFloat3("角度", &camera_->transform_.rotate_.x, 0.01f);
	ImGui::End();
	camera_->Update();

	ImGui::Begin("水");
	for (int i = 0; i < 3; i++) {
		std::string name = std::to_string(i);
		if (ImGui::TreeNode(name.c_str())) {
			ImGui::DragFloat3("ポジション", &water_[i]->position_.x, 0.01f);
			ImGui::DragFloat("サイズ", &water_[i]->scale_, 0.01f);
			ImGui::TreePop();
		}
	}
	ImGui::End();
#endif // _DEBUG

	WaveUpdate();

	for (int i = 0; i < 3; i++) {
		water_[i]->Update();
	}

	player_->Update(frameInfo_->GetDeltaTime());

	waveFloor_->Update();

	collisionManager_->CheckCollision();
}

void StageScene::Draw()
{
	instancingmodelManager_->Clear();

	Kyoko::Engine::PreDraw();

	player_->Draw(camera_.get());

	//waveFloor_->Draw();

	for (const std::unique_ptr<WaterChunk>& water : water_) {
		water->Draw();
	}

	instancingmodelManager_->Draw(*camera_.get());

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

