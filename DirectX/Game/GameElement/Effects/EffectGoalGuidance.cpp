#include "EffectGoalGuidance.h"

EffectGoalGuidance::EffectGoalGuidance()
{
	model_ = std::make_unique<Model>("plane");
}

EffectGoalGuidance::~EffectGoalGuidance()
{
}

void EffectGoalGuidance::Initialize(const Vector3& playerPos, const Vector3& goalPos, const float& goalSize)
{
	playerPos_ = &playerPos;
	goalPos_ = &goalPos;
	goalSize_ = &goalSize;
}

void EffectGoalGuidance::Update()
{
	//実際の境界
	Vector2 lineMax = {
		playerPos_->x + area_.x,
		playerPos_->y + area_.y
	};
	Vector2 lineMin = {
		playerPos_->x - area_.x,
		playerPos_->y - area_.y
	};

	if (lineMax.x <= goalPos_->x - *goalSize_ && lineMax.y <= goalPos_->y - *goalSize_ && lineMin.x >= goalPos_->x + *goalSize_ && lineMin.y >= goalPos_->y - *goalSize_) {
		isDraw_ = true;
	}
	else {
		//領域外で描画しない
		isDraw_ = false;
	}

	if (isDraw_) {
		//向きベクトル
		Vector3 direction = *goalPos_ - *playerPos_;
	}
}

void EffectGoalGuidance::Draw(const Camera* camera)
{

	if (isDraw_) {
		model_->Draw(*camera);
	}

}
