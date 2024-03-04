#pragma once
#include "Sprite.h"
#include <array>
#include <memory>
#include "GameElement/Score/Score.h"

class StageUI 
{
public:

	StageUI();

	void SetScore(const Score* score) { score_ = score; }
	void SetIsMusicFinish(const bool* isFinish) { isFinish_ = isFinish; }

	void Initialize();

	void Update();

	void Draw();

private:
	const Score* score_ = nullptr;
	const bool* isFinish_ = nullptr;

	const int kNumSize = 64;
	std::array<std::unique_ptr<Sprite>, 2> comboNum_;

	std::unique_ptr<Sprite> hitSp_;
	const int kHitSizeX_ = 256;
	const int kHitSizeY_ = 64;

	std::array<std::unique_ptr<Sprite>, 3> sp_;

	std::array<std::unique_ptr<Sprite>, 5> scoreNum_;

};
