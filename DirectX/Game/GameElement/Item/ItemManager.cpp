#include "ItemManager.h"
#include "Camera.h"

ItemManager* ItemManager::GetInstance()
{
	static ItemManager instance;
	return &instance;
}

void ItemManager::Clear()
{
	itemMap_.clear();
}

void ItemManager::InitializeGlobalVariables()
{
	stageEditor_ = std::make_unique<StageEditor>("アイテムの配置");
	globalVariable_ = std::make_unique<GlobalVariableUser>("Item", "StaticItem");
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	itemNum_ = 1;
	scale_ = 0.5f;
	SetGlobalVariable();
}

void ItemManager::Initialize()
{
	Clear();
	stageEditor_->Initialize();
	itemNum_ = 1;
	SetGlobalVariable();
	for (int i = 0; i < itemNum_; i++) {
		itemMap_[i] = std::make_unique<Item>(i, &scale_);
	}
}

void ItemManager::Update(float deltaTime, Camera* camera)
{
#ifdef _DEBUG
	ApplyGlobalVariable();

	if (stageEditor_->IsChangedStage()) {
		Initialize();
	}
#endif // _DEBUG

	for (int i = 0; i < itemNum_; i++) {
#ifdef _DEBUG
		if (itemMap_.find(i) == itemMap_.end()) {
			itemMap_[i] = std::make_unique<Item>(i, &scale_);
		}
#endif // _DEBUG
		itemMap_[i]->Update(deltaTime, camera);
	}
}

void ItemManager::Draw()
{
	for (int i = 0; i < itemNum_; i++) {
		itemMap_[i]->Draw();
	}
}

void ItemManager::SetGlobalVariable()
{
	stageEditor_->AddItem("アイテムの数", itemNum_);
	globalVariable_->AddItem("アイテムのスケール", scale_);
	globalVariable_->AddItem("アイテムの色", Vector3{ 1.0f,1.0f,1.0f });
	ApplyGlobalVariable();
}

void ItemManager::ApplyGlobalVariable()
{
	itemNum_ = stageEditor_->GetIntValue("アイテムの数");
	scale_ = globalVariable_->GetFloatValue("アイテムのスケール");
	Vector3 color = globalVariable_->GetVector3Value("アイテムの色");
	color_ = { color.x,color.y,color.z,1.0f };
	if (itemNum_ <= 0) {
		itemNum_ = 1;
	}
}
