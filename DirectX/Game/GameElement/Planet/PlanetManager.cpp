#include "PlanetManager.h"
#include "GameElement/Player/Player.h"
#include "ImGuiManager/ImGuiManager.h"
#include "RandomGenerator/RandomGenerator.h"

PlanetManager* PlanetManager::GetInstance()
{
	static PlanetManager instance;
	return &instance;
}

void PlanetManager::Initialize()
{
	num_ = 3;
	Clear();
	for (int i = 0; i < num_; i++) {
		planets_[i] = std::make_unique<Planet>(static_cast<PlanetType>(i), Vector3{}, player_, i);
	}
	time_ = 0.0f;
	minmax = { 2.0f,4.0f };
	rand_ = RandomGenerator::GetInstance();
	generateTime_ = rand_->GetInstance()->RandFloat(minmax.x, minmax.y);
	SetGlobalVariable();
}

void PlanetManager::Update(float deltaTime)
{
#ifdef _DEBUG
	ImGui::Begin("Planet");
	ImGui::SliderInt("惑星の数", &num_, 3, 15);
	ImGui::End();
	ApplyGlobalVariable();
#endif // _DEBUG

	time_ += deltaTime;

	for (int i = 0; i < num_; i++) {
		if (!planets_[i]) {
			PlanetType type = static_cast<PlanetType>(RandomGenerator::GetInstance()->RandInt(0, static_cast<int>(PlanetType::kEnd)));
			planets_[i] = std::make_unique<Planet>(type, Vector3{}, player_, i);
		}
		planets_[i]->Update(deltaTime);
	}

	CreateClient();
}

void PlanetManager::Draw()
{
	for (int i = 0; i < num_; i++) {
		planets_[i]->Draw();
	}
}

void PlanetManager::SetPlayer(Player* player)
{
	player_ = player;
}

void PlanetManager::SetGlobalVariable()
{
	globalVariable_ = std::make_unique<GlobalVariableUser>("Planet", "StaticPlanet");
	globalVariable_->AddItem("客を生成する最小と最大の時間", minmax);

	ApplyGlobalVariable();
}

void PlanetManager::ApplyGlobalVariable()
{
	minmax = globalVariable_->GetVector2Value("客を生成する最小と最大の時間");
}

void PlanetManager::CreateClient()
{
	if (time_ >= generateTime_) {
		generateTime_ = rand_->RandFloat(minmax.x, minmax.y);
		time_ = 0.0f;

		int num = rand_->RandInt(0, num_);

		planets_[num]->CreateClient();
	}
}
