#pragma once
#include <memory>
#include <unordered_map>
#include "Item.h"
#include "StageEditor/StageEditor.h"

class Camera;

class ItemManager {
public:
	static ItemManager* GetInstance();

	void Clear();

	void InitializeGlobalVariables();

	void Initialize();

	void Update(float deltaTime, Camera* camera);

	void Draw();

	const Vector4& GetColor() const { return color_; }

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
	std::unique_ptr<GlobalVariableUser> globalVariable_;

	float scale_;
	int itemNum_;
	Vector4 color_;
};