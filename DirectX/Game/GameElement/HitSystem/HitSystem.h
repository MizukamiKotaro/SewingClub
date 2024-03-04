#pragma once
#include <memory>
#include "GameElement/Score/Score.h"
#include "Audio.h"

enum HitEvaluation {
	PERFECT,
	GREAT,
	GOOD,
	MISS,
};

class HitSystem {
public: 

	static void SetScore(Score* score) { score_ = score; }

	static void Initialize();

	static void Update(float time);

	static int* GetHitCount() { return &hitCount_; }

	static int GetHitNum() { return hitNum_; }

private:

	static void HitTest(float time);

private:
	
	static Score* score_;

	static int hitCount_;
	static int hitNum_;

	static const float kHitErrorFrame_[3];

	static Audio se_;
};
