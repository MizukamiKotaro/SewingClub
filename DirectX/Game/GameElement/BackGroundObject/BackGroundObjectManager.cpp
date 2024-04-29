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
	mistNum_ = 0;
	SetGlobalVariable();
}

void BackGroundObjectManager::Initialize()
{
	Clear();
	stageEditor_->Initialize();
	cloudNum_ = 0;
	mistNum_ = 0;
	SetGlobalVariable();
	for (int i = 0; i < cloudNum_; i++) {
		cloudMap_[i] = std::make_unique<BackGroundObject>(i,BackGroundObjectType::CLOUD);
	}
	for (int i = 0; i < mistNum_; i++) {
		mistMap_[i] = std::make_unique<BackGroundObject>(i, BackGroundObjectType::MIST);
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

	for (int i = 0; i < mistNum_; i++) {
#ifdef _DEBUG
		if (mistMap_.find(i) == mistMap_.end()) {
			mistMap_[i] = std::make_unique<BackGroundObject>(i, BackGroundObjectType::MIST);
		}
#endif // _DEBUG
		mistMap_[i]->Update(deltaTime);
	}
}

void BackGroundObjectManager::Draw()
{
	for (int i = 0; i < cloudNum_; i++) {
		cloudMap_[i]->Draw();
	}
	for (int i = 0; i < mistNum_; i++) {
		mistMap_[i]->Draw();
	}
}

void BackGroundObjectManager::SetGlobalVariable()
{
	stageEditor_->AddItem("雲の数", cloudNum_, "雲の配置");
	stageEditor_->AddItem("もやの数", mistNum_, "もやの配置");
	ApplyGlobalVariable();
}

void BackGroundObjectManager::ApplyGlobalVariable()
{
	cloudNum_ = stageEditor_->GetIntValue("雲の数", "雲の配置");
	mistNum_ = stageEditor_->GetIntValue("もやの数",  "もやの配置");
	if (cloudNum_ < 0) {
		cloudNum_ = 0;
	}
	if (mistNum_ < 0) {
		mistNum_ = 0;
	}
}
