#include "WaterManager.h"

WaterManager* WaterManager::GetInstance()
{
	static WaterManager instance;
	return &instance;
}

void WaterManager::Clear()
{
	fullWater_.clear();
	stageWater_.clear();
}

void WaterManager::InitializeGlobalVariables()
{
	stageEditor_ = std::make_unique<StageEditor>("水の配置");
	SetGlobalVariable();
}

void WaterManager::Initialize()
{
	Clear();
	stageEditor_->Initialize();
	waterNum_ = 1;
	SetGlobalVariable();
	for (int i = 0; i < waterNum_; i++) {
		stageWater_[i] = std::make_unique<WaterChunk>(i);
	}
}

void WaterManager::Update(float deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();

	if (stageEditor_->IsChangedStage()) {
		Initialize();
	}
#endif // _DEBUG

	for (int i = 0; i < waterNum_; i++) {
#ifdef _DEBUG
		if (stageWater_.find(i) == stageWater_.end()) {
			stageWater_[i] = std::make_unique<WaterChunk>(i);
		}
#endif // _DEBUG
		stageWater_[i]->Update(deltaTime);
	}

	for (std::list<std::unique_ptr<WaterChunk>>::iterator it = fullWater_.begin(); it != fullWater_.end();) {
		if ((*it)->IsDelete()) {
			(*it).reset(nullptr);
			it = fullWater_.erase(it);
		}
		else {
			(*it)->Update(deltaTime);
			it++;
		}
	}


}

void WaterManager::Draw()
{
	for (std::unique_ptr<WaterChunk>& water : fullWater_) {
		water->Draw();
	}
	for (int i = 0; i < waterNum_; i++) {
		stageWater_[i]->Draw();
	}
}

void WaterManager::CreateWater(const Vector2& pos, const Vector2& radius, bool isSame, const float& rotate, bool isSmall)
{
	fullWater_.push_back(std::make_unique<WaterChunk>(pos, radius, isSame, rotate, isSmall));
}

void WaterManager::SetGlobalVariable()
{
	stageEditor_->AddItem("水の数", waterNum_);
	ApplyGlobalVariable();
}

void WaterManager::ApplyGlobalVariable()
{
	waterNum_ = stageEditor_->GetIntValue("水の数");
	if (waterNum_ <= 0) {
		waterNum_ = 1;
	}
}
