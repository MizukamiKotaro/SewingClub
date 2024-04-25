#pragma once
#include <list>
#include <memory>
#include <unordered_map>
#include "GameElement/WaterChunk/WaterChunk.h"
#include "StageEditor/StageEditor.h"

class Camera;

class WaterManager {
public:
	static WaterManager* GetInstance();

	void Clear();

	void InitializeGlobalVariables();

	void Initialize();

	void Update(const float& deltaTime, Camera* camera);

	void Draw();

	void CreateWater(const Vector2& pos, const Vector2& radius, bool isSame, const float& rotate, bool isSmall = true);

	std::unordered_map<int, std::unique_ptr<WaterChunk>>& GetWater() { return stageWater_; }

private:
	WaterManager() = default;
	~WaterManager() = default;
	WaterManager(const WaterManager&) = delete;
	WaterManager& operator=(const WaterManager&) = delete;

	// グローバル変数のセット
	void SetGlobalVariable();
	// グローバル変数の更新
	void ApplyGlobalVariable();

private:
	std::list<std::unique_ptr<WaterChunk>> fullWater_;
	std::unordered_map<int, std::unique_ptr<WaterChunk>> stageWater_;
	std::unique_ptr<StageEditor> stageEditor_;

	int waterNum_;

};