#include "Score.h"

void Score::Reset() {

	for (int i = 0; i < END_PARAMETER; i++) {
		parameters_[i] = 0;
	}
	isFullCom_ = false;
	isAP_ = false;
}

void Score::ComboUpdate() {
	if (parameters_[HIGH_COMBO] < parameters_[COMBO]) {
		parameters_[HIGH_COMBO] = parameters_[COMBO];
	}
}

void Score::SetUpdate(const Score& score) {
	
	if (parameters_[SCORE] < score.parameters_[SCORE]) {
		parameters_[SCORE] = score.parameters_[SCORE];
	}

	if (parameters_[HIGH_COMBO] < score.parameters_[HIGH_COMBO]) {
		parameters_[HIGH_COMBO] = score.parameters_[HIGH_COMBO];
	}

	if (!isFullCom_) {
		isFullCom_ = score.isFullCom_;
	}

	if (!isAP_) {
		isAP_ = score.isAP_;
	}
}

void Score::SetScore(const Score& score)
{
	parameters_ = score.parameters_;
}

void Score::IsFullComUpdate() {
	if (parameters_[MISS] == 0 && parameters_[GREAT] == 0) {
		isFullCom_ = true;
		if (parameters_[GREAT] == 0) {
			isAP_ = true;
		}
		else {
			isAP_ = false;
		}
	} else {
		isFullCom_ = false;
		isAP_ = false;
	}
}

void Score::AddCom() {
	parameters_[COMBO]++;
	ComboUpdate();
}