#pragma once
#include <list>
#include <memory>
#include "GameElement/WaterChunk/WaterChunk.h"

class WaterManager {
public:
	static WaterManager* GetInstance();

	void Clear();

	void Update();

	void Draw();

	void CreateWater(const Vector2& pos, const Vector2& radius, bool isSame, const float& rotate);

private:
	WaterManager() = default;
	~WaterManager() = default;
	WaterManager(const WaterManager&) = delete;
	WaterManager& operator=(const WaterManager&) = delete;

private:
	std::list<std::unique_ptr<WaterChunk>> fullWater_;

};