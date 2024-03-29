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
}

void PlanetManager::Update(float deltaTime)
{
#ifdef _DEBUG
	ImGui::Begin("Planet");
	ImGui::SliderInt("惑星の数", &num_, 3, 15);
	ImGui::End();
#endif // _DEBUG

	for (int i = 0; i < num_; i++) {
		if (!planets_[i]) {
			PlanetType type = static_cast<PlanetType>(RandomGenerator::GetInstance()->RandInt(0, static_cast<int>(PlanetType::kEnd)));
			planets_[i] = std::make_unique<Planet>(type, Vector3{}, player_, i);
		}
		planets_[i]->Update(deltaTime);
	}
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
