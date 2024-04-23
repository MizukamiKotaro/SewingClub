#pragma once
#include <memory>
#include <unordered_map>
#include "BackGroundObject.h"
#include "StageEditor/StageEditor.h"

enum class BackGroundObjectType;

class BackGroundObjectManager {
public:
	static BackGroundObjectManager* GetInstance();

	void Clear();

	void InitializeGlobalVariables();

	void Initialize();

	void Update(float deltaTime);

	void Draw();

private:
	BackGroundObjectManager() = default;
	~BackGroundObjectManager() = default;
	BackGroundObjectManager(const BackGroundObjectManager&) = delete;
	BackGroundObjectManager& operator=(const BackGroundObjectManager&) = delete;

	// グローバル変数のセット
	void SetGlobalVariable();
	// グローバル変数の更新
	void ApplyGlobalVariable();

private:
	std::unique_ptr<StageEditor> stageEditor_;
	std::unordered_map<int, std::unique_ptr<BackGroundObject>> cloudMap_;
	std::unordered_map<int, std::unique_ptr<BackGroundObject>> mistMap_;

	int cloudNum_;
	int mistNum_;
};