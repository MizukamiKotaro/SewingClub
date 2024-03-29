#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include "Planet.h"

class Player;

class PlanetManager
{
public:

	static PlanetManager* GetInstance();

	void Clear() { planets_.clear(); }

	void Initialize();

	void Update(float deltaTime);

	void Draw();

	void SetPlayer(Player* player);

private:
	PlanetManager() = default;
	~PlanetManager() = default;
	PlanetManager(const PlanetManager&) = delete;
	PlanetManager& operator=(const PlanetManager&) = delete;

private:
	int num_ = 0;
	std::unordered_map<int, std::unique_ptr<Planet>> planets_;
	Player* player_ = nullptr;
};