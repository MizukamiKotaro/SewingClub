#include "GoalCamera.h"
#include "Utils/Ease/Ease.h"
#include "Input/Input.h"

GoalCamera::GoalCamera() {

}

Vector3 GoalCamera::Update(const Vector3& playerPos, const Vector3& goalPos, const float& delta) {
	Vector3 result{};
	bool isSwitch = false;

	switch (type_) {
	case GoalCamera::TYPE::ToGoal:
		result = Ease::UseEase(playerPos, goalPos, count_, kMaxFrame_, Ease::EaseType::EaseIn);
		if (kMaxFrame_ <= count_) {
			isSwitch = true;
		}
		break;
	case GoalCamera::TYPE::Wait:
		result = goalPos;
		if (kWaitTime_ <= count_) {
			//isSwitch = true;
		}
		break;
	case GoalCamera::TYPE::ToPlayer:
		result = Ease::UseEase(goalPos, playerPos, count_, kMaxFrame_, Ease::EaseType::EaseIn);
		if (kMaxFrame_ <= count_) {
			isSwitch = true;
		}
		break;
	}

	if (isSwitch) {
		int index = static_cast<int>(type_);
		type_ = static_cast<TYPE>(index + 1);
		count_ = 0.0f;
	}
	else {
		// 入力があれば速くする
		auto input = Input::GetInstance();
		float magnification = 1.0f;
		if (input->PressedGamePadButton(Input::GamePadButton::A)) {
			isSkiped = true;
		}
		else if (input->PressedGamePadButton(Input::GamePadButton::B)) {
			isSkiped = true;
		}

		if (isSkiped) {
			magnification = 3.0f;
		}
		count_ += delta * magnification;
	}
	return result;
}

bool GoalCamera::GetFinishd() {
	bool result = false;
	if (type_ == TYPE::kMaxNum) {
		result = true;
		type_ = TYPE::ToGoal;
	}
	return result;
}

void GoalCamera::SetNext() {
	int index = static_cast<int>(type_);
	type_ = static_cast<TYPE>(index + 1);
	count_ = 0.0f;
}
