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
	reqItemMap_.clear();
}

void ItemManager::InitializeGlobalVariables()
{
	stageEditor_ = std::make_unique<StageEditor>("アイテムの配置");
	globalVariable_ = std::make_unique<GlobalVariableUser>("Item", "StaticItem");
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	itemNum_ = 0;
	scale_ = 0.5f;
	reqItemNum_ = 0;
	reqScale_ = 0.8f;
	reqScaleDiameter_ = 3.0f;
	SetGlobalVariable();
}

void ItemManager::Initialize()
{
	Clear();
	stageEditor_->Initialize();
	itemNum_ = 0;
	reqItemNum_ = 0;
	SetGlobalVariable();
	for (int i = 0; i < itemNum_; i++) {
		itemMap_[i] = std::make_unique<Item>(i, &scale_);
	}
	for (int i = 0; i < reqItemNum_; i++) {
		reqItemMap_[i] = std::make_unique<RequiredObject>(i, reqScale_, reqScaleDiameter_);
	}
	isCanGoal_ = false;
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

	uint32_t count = 0;
	for (int i = 0; i < reqItemNum_; i++) {
#ifdef _DEBUG
		if (reqItemMap_.find(i) == reqItemMap_.end()) {
			reqItemMap_[i] = std::make_unique<RequiredObject>(i, reqScale_, reqScaleDiameter_);
		}
#endif // _DEBUG
		if (reqItemMap_[i]->Update(deltaTime, camera)) {
			count++;
		}
	}
	// 必須アイテムが置かれている分取得できたら
	if (count >= static_cast<uint32_t>(reqItemNum_)) {
		// ゴール描画
		isCanGoal_ = true;
	}
}

void ItemManager::Draw()
{
	for (int i = 0; i < itemNum_; i++) {
		itemMap_[i]->Draw();
	}
	for (int i = 0; i < reqItemNum_; i++) {
		reqItemMap_[i]->Draw();
	}
}

std::list<QuotaSendData> ItemManager::GetQuotaData()
{
	std::list<QuotaSendData>datas;

	for (auto& data : reqItemMap_) {
		QuotaSendData newdata;
		newdata = data.second->GetQuotaData();
		datas.push_back(newdata);
	}
	return datas;
}

void ItemManager::SetGlobalVariable()
{
	stageEditor_->AddItem("アイテムの数", itemNum_);
	globalVariable_->AddItem("アイテムのスケール", scale_);
	globalVariable_->AddItem("アイテムの色", Vector3{ 1.0f,1.0f,1.0f });
	stageEditor_->AddItem("必須アイテムの数", reqItemNum_);
	globalVariable_->AddItem("必須アイテムのスケール", reqScale_);
	globalVariable_->AddItem("必須アイテムのコライダーの倍率", reqScaleDiameter_);

	ApplyGlobalVariable();
}

void ItemManager::ApplyGlobalVariable()
{
	itemNum_ = stageEditor_->GetIntValue("アイテムの数");
	scale_ = globalVariable_->GetFloatValue("アイテムのスケール");
	Vector3 color = globalVariable_->GetVector3Value("アイテムの色");
	color_ = { color.x,color.y,color.z,1.0f };
	if (itemNum_ < 0) {
		itemNum_ = 0;
	}

	reqItemNum_ = stageEditor_->GetIntValue("必須アイテムの数");
	reqScale_ = globalVariable_->GetFloatValue("必須アイテムのスケール");
	reqScaleDiameter_ = globalVariable_->GetFloatValue("必須アイテムのコライダーの倍率");
	if (reqItemNum_ < 0) {
		reqItemNum_ = 0;
	}
}
