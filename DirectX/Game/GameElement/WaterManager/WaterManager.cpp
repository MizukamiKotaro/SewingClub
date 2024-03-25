#include "WaterManager.h"

WaterManager* WaterManager::GetInstance()
{
	static WaterManager instance;
	return &instance;
}

void WaterManager::Clear()
{
	fullWater_.clear();
}

void WaterManager::Update()
{
	for (std::unique_ptr<WaterChunk>& water : fullWater_) {
		water->Update();
	}
}

void WaterManager::Draw()
{
	for (std::unique_ptr<WaterChunk>& water : fullWater_) {
		water->Draw();
	}
}

void WaterManager::CreateWater(const Vector2& pos, const Vector2& radius, bool isSame, const float& rotate)
{
	fullWater_.push_back(std::make_unique<WaterChunk>(pos, radius, isSame, rotate));
}
