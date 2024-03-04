#pragma once
#include "Sprite.h"
#include <array>
#include <memory>
#include "GameElement/Score/Score.h"

class ClearUI
{
public:
	ClearUI();

	//void SetScore(const Score* score) { score_ = score; }

	void Initialize();

	void Update();

	void Draw();

	const bool IsEnd() const { return isEnd_; }

private:

	const Score* score_ = nullptr;

	const int kNumSize = 64;
	const int kHitSizeX_ = 256;
	const int kHitSizeY_ = 64;

	std::array<std::unique_ptr<Sprite>, 5> scoreNum_;

	std::array<std::unique_ptr<Sprite>, 6> sp_;
	std::array<std::array<std::unique_ptr<Sprite>, 2>, 5> numSp_;

	std::array<bool, 6> isDraws_;

	float time_;
	bool isEnd_;
};

