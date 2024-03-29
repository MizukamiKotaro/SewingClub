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

void WaterManager::Update(float deltaTime)
{
	for (std::unique_ptr<WaterChunk>& water : fullWater_) {
		water->Update(deltaTime);
	}
}

void WaterManager::Draw()
{
	for (std::unique_ptr<WaterChunk>& water : fullWater_) {
		water->Draw();
	}
}

void WaterManager::CreateWater(const Vector2& pos, const Vector2& radius, bool isSame, const float& rotate, bool isSmall)
{
	fullWater_.push_back(std::make_unique<WaterChunk>(pos, radius, isSame, rotate, isSmall));
}
