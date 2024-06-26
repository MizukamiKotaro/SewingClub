#pragma once
#include <list>
#include <memory>
#include "IEnemy.h"
#include"GameElement/Effects/UIEnemyGuidance/UIEnemyGuidance.h"

class Player;

class EnemyManager {
public:
	static EnemyManager* GetInstance();

	void Clear() { enemies_.clear(); }

	void Initialize(Player* pplayer, const Camera* camera);

	void Update(const float& deltaTime, Camera* camera, const uint32_t& babyTension);

	void Draw() const;

private:
	EnemyManager() = default;
	~EnemyManager() = default;
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;

	void SetGlobalVariable();

	void ApplyGlobalVariable();

	void GenerateEnemies();

private:
	std::list<std::unique_ptr<IEnemy>> enemies_;

	std::unique_ptr<StageEditor> stageEditor_;
	int doesNotWorkEnemyNum_ = 0;
	int followEnemyNum_ = 0;

	Player* player_ptr = nullptr; // playerのポインタ

	std::unique_ptr<UIEnemyGuidance>guid_;
};