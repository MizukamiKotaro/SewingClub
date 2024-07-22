#pragma once
#include <memory>
#include <unordered_map>
#include "Item.h"
#include "RequiredObject.h"
#include "StageEditor/StageEditor.h"
#include "GameElement/Gimmick/MoveWaterGimmick.h"

class Camera;

class ItemManager {
public:
	static ItemManager* GetInstance();

	static void StaticInitialize();

	void Clear();

	void InitializeGlobalVariables();

	void Initialize();

	void Update(float deltaTime, Camera* camera);

	void Draw();

	const Vector4& GetColor() const { return color_; }
	const bool& GetIsCanGoal() const { return isCanGoal_; }

	std::list<QuotaSendData>GetQuotaData();
private:
	ItemManager() = default;
	~ItemManager() = default;
	ItemManager(const ItemManager&) = delete;
	ItemManager& operator=(const ItemManager&) = delete;

	// グローバル変数のセット
	void SetGlobalVariable();
	// グローバル変数の更新
	void ApplyGlobalVariable();

private:
	std::unique_ptr<StageEditor> stageEditor_;
	std::unordered_map<int, std::unique_ptr<Item>> itemMap_;
	std::unordered_map<int, std::unique_ptr<RequiredObject>> reqItemMap_;
	std::unordered_map<int, std::unique_ptr<MoveWaterGimmick>> moveWaterGimmick_;
	std::unique_ptr<GlobalVariableUser> globalVariable_;

	float scale_;
	int itemNum_;
	float reqScale_ = 1.0f;
	float reqScaleDiameter_ = 1.0f;
	int reqItemNum_ = 1;
	Vector4 color_;

	int moveWaterGimmickNum_ = 0;

	bool isCanGoal_ = false;
};