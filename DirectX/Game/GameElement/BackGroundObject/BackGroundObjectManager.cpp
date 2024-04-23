#include "BackGroundObjectManager.h"
#include "BackGroundObjectConfig.h"

BackGroundObjectManager* BackGroundObjectManager::GetInstance()
{
	static BackGroundObjectManager instance;
	return &instance;
}

void BackGroundObjectManager::Clear()
{
	cloudMap_.clear();
}

void BackGroundObjectManager::InitializeGlobalVariables()
{
	stageEditor_ = std::make_unique<StageEditor>("背景オブジェクトの配置");
	cloudNum_ = 0;
	SetGlobalVariable();
}

void BackGroundObjectManager::Initialize()
{
	Clear();
	stageEditor_->Initialize();
	cloudNum_ = 0;
	SetGlobalVariable();
	for (int i = 0; i < cloudNum_; i++) {
		cloudMap_[i] = std::make_unique<BackGroundObject>(i,BackGroundObjectType::CLOUD);
	}
}

void BackGroundObjectManager::Update(float deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();

	if (stageEditor_->IsChangedStage()) {
		Initialize();
	}
#endif // _DEBUG

	for (int i = 0; i < cloudNum_; i++) {
#ifdef _DEBUG
		if (cloudMap_.find(i) == cloudMap_.end()) {
			cloudMap_[i] = std::make_unique<BackGroundObject>(i, BackGroundObjectType::CLOUD);
		}
#endif // _DEBUG
		cloudMap_[i]->Update(deltaTime);
	}
}

void BackGroundObjectManager::Draw()
{
	for (int i = 0; i < cloudNum_; i++) {
		cloudMap_[i]->Draw();
	}
}

void BackGroundObjectManager::SetGlobalVariable()
{
	stageEditor_->AddItem("雲の数", cloudNum_, "雲の配置");
	ApplyGlobalVariable();
}

void BackGroundObjectManager::ApplyGlobalVariable()
{
	cloudNum_ = stageEditor_->GetIntValue("雲の数", "雲の配置");
	if (cloudNum_ < 0) {
		cloudNum_ = 0;
	}
}
