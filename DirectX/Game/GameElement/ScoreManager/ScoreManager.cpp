#include "ScoreManager.h"

ScoreManager* ScoreManager::GetInstance()
{
	static ScoreManager instance;
	return &instance;
}

void ScoreManager::Initialize()
{
	for (int i = 0; i < STAGE::COUNT_STAGE; i++) {
		memoryScores_[i] = std::make_unique<Score>();
	}

	clearScore_ = std::make_unique<Score>();
}

void ScoreManager::UpdateMemory()
{
	memoryScores_[IScene::stageNo_]->SetUpdate(*clearScore_.get());
}
