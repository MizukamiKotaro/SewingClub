#include "ItemManager.h"

ItemManager* ItemManager::GetInstance()
{
	static ItemManager instance;
	return &instance;
}

void ItemManager::Clear()
{
	ItemMap_.clear();
}

void ItemManager::InitializeGlobalVariables()
{
	stageEditor_ = std::make_unique<StageEditor>("アイテムの配置");
	itemNum_ = 1;
	SetGlobalVariable();
}

void ItemManager::Initialize()
{
	Clear();
	stageEditor_->Initialize();
	itemNum_ = 1;
	SetGlobalVariable();
	for (int i = 0; i < itemNum_; i++) {
		ItemMap_[i] = std::make_unique<Item>(i);
	}
}

void ItemManager::Update(float deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();

	if (stageEditor_->IsChangedStage()) {
		Initialize();
	}
#endif // _DEBUG

	for (int i = 0; i < itemNum_; i++) {
#ifdef _DEBUG
		if (ItemMap_.find(i) == ItemMap_.end()) {
			ItemMap_[i] = std::make_unique<Item>(i);
		}
#endif // _DEBUG
		ItemMap_[i]->Update(deltaTime);
	}
}

void ItemManager::Draw()
{
	for (int i = 0; i < itemNum_; i++) {
		ItemMap_[i]->Draw();
	}
}

void ItemManager::SetGlobalVariable()
{
	stageEditor_->AddItem("アイテムの数", itemNum_);
	ApplyGlobalVariable();
}

void ItemManager::ApplyGlobalVariable()
{
	itemNum_ = stageEditor_->GetIntValue("アイテムの数");
	if (itemNum_ <= 0) {
		itemNum_ = 1;
	}
}
