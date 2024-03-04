#include "ClearUI.h"
#include "GameElement/ScoreManager/ScoreManager.h"
#include "Input.h"
#include "FrameInfo/FrameInfo.h"

ClearUI::ClearUI()
{
	for (int i = 0; i < 6; i++) {
		if (i < 5) {
			scoreNum_[i] = std::make_unique<Sprite>("scoreNum.png");
			scoreNum_[i]->pos_ = { float(i * 64.0f + 600.0f), 500.0f };
			scoreNum_[i]->size_ = { 64.0f, 64.0f };
			scoreNum_[i]->SetTextureSize({ 64.0f, 64.0f });
			scoreNum_[i]->Update();

			for (int j = 0; j < 2; j++) {
				numSp_[i][j] = std::make_unique<Sprite>("scoreNum.png");
				numSp_[i][j]->pos_ = { float(j * 64.0f + 792.0f), 100.0f + i * 80.0f };
				numSp_[i][j]->size_ = { 64.0f, 64.0f };
				numSp_[i][j]->SetTextureSize({ 64.0f, 64.0f });
				numSp_[i][j]->Update();
			}
		}

		if (i < 4) {
			sp_[i] = std::make_unique<Sprite>("hit.png");
			sp_[i]->SetTextureTopLeft({ 256.0f * i, 0.0f });
		}
		else {
			sp_[i] = std::make_unique<Sprite>("gameScene.png");
			sp_[i]->SetTextureTopLeft({ -256.0f * (i - 4) + 256.0f, 0.0f });
		}
		sp_[i]->pos_ = { 400.0f, 100.0f + i * 80.0f };
		sp_[i]->size_ = { 256.0f, 64.0f };
		sp_[i]->SetTextureSize({ 256.0f, 64.0f });
		sp_[i]->Update();

		isDraws_[i] = false;
	}

	time_ = 0.0f;
	isEnd_ = false;
}

void ClearUI::Initialize()
{
	score_ = ScoreManager::GetInstance()->GetClearScore();
	int nums[6];
	nums[0] = score_->GetPer();
	nums[1] = score_->GetGre();
	nums[2] = score_->GetGood();
	nums[3] = score_->GetMiss();
	nums[4] = score_->GetHiCombo();
	nums[5] = score_->GetScore();

	for (int i = 0; i < 6; i++) {
		if (i < 5) {
			int num = nums[5] * int(pow(10, i)) / 10000;
			nums[5] = nums[5] % (10000 / int(pow(10, i)));

			if (num > 9) {
				num = 0;
			}
			scoreNum_[i]->SetTextureTopLeft({ float(num * 64.0f), 0.0f });
			scoreNum_[i]->Update();

			for (int j = 0; j < 2; j++) {
				int hoge= nums[i] * int(pow(10, j)) / 10;
				nums[i] = nums[i] % (10 / int(pow(10, j)));

				if (hoge > 9) {
					hoge = 0;
				}
				numSp_[i][j]->SetTextureTopLeft({ float(hoge * 64.0f), 0.0f });
				numSp_[i][j]->Update();
			}
		}

		isDraws_[i] = false;
	}

	time_ = 0.0f;
	isEnd_ = false;
}

void ClearUI::Update()
{
	float interval = 0.2f;

	time_ += FrameInfo::GetInstance()->GetDeltaTime();

	for (int i = 0; i < 6; i++) {
		if (time_ >= interval + interval * i) {
			if (!isDraws_[i]) {
				isDraws_[i] = true;
			}
		}
	}

	if (time_ >= interval * 7 || Input::GetInstance()->PressedGamePadButton(Input::GamePadButton::A)) {
		if (Input::GetInstance()->PressedGamePadButton(Input::GamePadButton::A) && !isEnd_) {

		}
		for (int i = 0; i < 6; i++) {
			isDraws_[i] = true;
		}

		isEnd_ = true;
	}
}

void ClearUI::Draw()
{
	for (int i = 0; i < 6; i++) {
		if (i < 5) {
			if (isDraws_[5]) {
				scoreNum_[i]->Draw();
			}

			if (isDraws_[i]) {
				for (int j = 0; j < 2; j++) {
					numSp_[i][j]->Draw();
				}
			}
		}

		sp_[i]->Draw();
	}
}
