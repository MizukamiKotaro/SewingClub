#include "WaterManager.h"
#include "Camera.h"

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
	
	for (int i = 0; i < waterNum_; i++) {
		stageWater_[i]->CreateQuadrangle();
		
		// 上限Xの探索
		float handle = stageWater_[i]->GetPosition().x + stageWater_[i]->GetScale();
		if (handle > limit_.upperLimit.x) {
			limit_.upperLimit.x = handle;
		}
		// 下限Xの探索
		handle = stageWater_[i]->GetPosition().x - stageWater_[i]->GetScale();
		if (handle < limit_.lowerLimit.x) {
			limit_.lowerLimit.x = handle;
		}
		// 上限Yの探索
		handle = stageWater_[i]->GetPosition().y + stageWater_[i]->GetScale();
		if (handle > limit_.upperLimit.y) {
			limit_.upperLimit.y = handle;
		}
		// 下限Yの探索
		handle = stageWater_[i]->GetPosition().y - stageWater_[i]->GetScale();
		if (handle < limit_.lowerLimit.y) {
			limit_.upperLimit.y = handle;
		}
	}
}

void WaterManager::Update(const float& deltaTime, Camera* camera)
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
		stageWater_[i]->Update(deltaTime, camera);
	}

	for (std::list<std::unique_ptr<WaterChunk>>::iterator it = fullWater_.begin(); it != fullWater_.end();) {
		if ((*it)->IsDelete()) {
			(*it).reset(nullptr);
			it = fullWater_.erase(it);
		}
		else {
			(*it)->Update(deltaTime, camera);
			it++;
		}
	}


}

void WaterManager::Draw(Camera* camera)
{
	for (std::unique_ptr<WaterChunk>& water : fullWater_) {
		water->Draw(camera);
	}
	for (int i = 0; i < waterNum_; i++) {
		stageWater_[i]->Draw(camera);
	}
}

void WaterManager::CreateWater(const Vector2& pos, const Vector2& radius, bool isSame, const float& rotate, bool isSmall)
{
	fullWater_.push_back(std::make_unique<WaterChunk>(pos, radius, isSame, rotate, isSmall));
}

const WaterChunk* WaterManager::GetWater(const int& no)
{
	if (stageWater_.find(no) != stageWater_.end()) {
		return stageWater_[no].get();
	}

	return nullptr;
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
