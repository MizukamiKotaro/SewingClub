#pragma once

#include "GameElement/Score/Score.h"
#include "SceneSystem/IScene/IScene.h"
#include <array>
#include <memory>

class ScoreManager
{
public:
	static ScoreManager* GetInstance();

	void Initialize();

	void UpdateMemory();

	void SetClearScore(Score* score) { clearScore_->SetScore(*score); }

	Score* GetClearScore() { return clearScore_.get(); }

	std::array<std::unique_ptr<Score>, STAGE::COUNT_STAGE>* GetMemoryScores() { return &memoryScores_; }

private:
	ScoreManager() = default;
	~ScoreManager() = default;
	ScoreManager(const ScoreManager&) = delete;
	ScoreManager& operator=(const ScoreManager&) = delete;

private:

	std::array<std::unique_ptr<Score>, STAGE::COUNT_STAGE> memoryScores_;

	std::unique_ptr<Score> clearScore_;
};