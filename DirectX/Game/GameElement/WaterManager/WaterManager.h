#pragma once
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>
#include "GameElement/WaterChunk/WaterChunk.h"
#include "StageEditor/StageEditor.h"
#include "GameElement/Item/ItemManager.h"

class Camera;

class WaterManager {
public:
	static WaterManager* GetInstance();

	void Clear();

	void InitializeGlobalVariables();

	void Initialize();

	void Update(const float& deltaTime, Camera* camera);

	void Draw(Camera* camera);

	void CreateWater(const Vector2& pos, const Vector2& radius, bool isSame, const float& rotate, bool isSmall = true);

	void CreateGimmickWater(std::list<MoveWaterGimmick::GimmickWaterParam> list);

	std::unordered_map<int, std::unique_ptr<WaterChunk>>& GetWater() { return stageWater_; }
	const WaterChunk* GetWater(const int& no);

	void AddWater(const Vector2& pos);

private:
	WaterManager() = default;
	~WaterManager() = default;
	WaterManager(const WaterManager&) = delete;
	WaterManager& operator=(const WaterManager&) = delete;

	// グローバル変数のセット
	void SetGlobalVariable();
	// グローバル変数の更新
	void ApplyGlobalVariable();

	bool LimitCheck(const Vector3& position, const float& scale) const;

private:
	std::list<std::unique_ptr<WaterChunk>> fullWater_;
	std::vector<std::unique_ptr<WaterChunk>> gimmickWater_;
	std::unordered_map<int, std::unique_ptr<WaterChunk>> stageWater_;
	std::unique_ptr<StageEditor> stageEditor_;

	int waterNum_;

	struct Limit {
		Vector3 upperLimit{}; // 右上
		Vector3 lowerLimit{}; // 左下
	};
	Limit limit_;
public:
	const Limit& GetLimit() const { return limit_; }

};