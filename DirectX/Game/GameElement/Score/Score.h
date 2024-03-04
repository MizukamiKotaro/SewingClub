#pragma once

#include <array>

class Score {

public:

	enum Parameter {
		SCORE,
		COMBO,
		HIGH_COMBO,
		PERFECT,
		GREAT,
		GOOD,
		MISS,
		END_PARAMETER,
	};

	void Reset();

	void ComboUpdate();

	void SetUpdate(const Score& score);

	void SetScore(const Score& score);

	void AddScore(int score) { parameters_[SCORE] += score; }

	void AddPer() {
		parameters_[PERFECT]++;
		AddCom();
		AddScore(200 * (parameters_[COMBO] / 10 + 1));
	}

	void AddGre() {
		parameters_[GREAT]++;
		AddCom();
		AddScore(100 * (parameters_[COMBO] / 10 + 1));
	}

	void AddGood() {
		parameters_[GOOD]++;
		ComboReset();
		AddScore(50);
	}

	void AddMiss() {
		parameters_[MISS]++;
		ComboReset();
	}

	void AddCom();

	void ComboReset() { parameters_[COMBO] = 0; }

	void IsFullComUpdate();

	const int GetScore() const { return parameters_[SCORE]; }

	const int GetCom() const { return parameters_[COMBO]; }

	const int GetHiCombo() const { return parameters_[HIGH_COMBO]; }

	const int GetPer() const { return parameters_[PERFECT]; }

	const int GetGre() const { return parameters_[GREAT]; }

	const int GetGood() const { return parameters_[GOOD]; }

	const int GetMiss() const { return parameters_[MISS]; }

	const bool IsFullCom() const { return isFullCom_; }

	const bool IsAP() const { return isAP_; }

private:

	/*int score_ = 0;
	int combo_ = 0;
	int perfectNum_ = 0;
	int greatNum_ = 0;
	int goodNum_ = 0;
	int missNum_ = 0;
	int hiCombo_ = 0;*/

	std::array<int, END_PARAMETER> parameters_;

	bool isFullCom_ = false;
	bool isAP_ = false;
};
