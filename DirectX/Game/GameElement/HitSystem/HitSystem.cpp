#include "HitSystem.h"
#include "GameElement/Notes/Notes.h"
#include "Input.h"

Score* HitSystem::score_ = nullptr;
int HitSystem::hitCount_ = 0;
int HitSystem::hitNum_ = 0;

Audio HitSystem::se_;

const float HitSystem::kHitErrorFrame_[3] = { 0.1f, 0.3f, 0.6f };

void HitSystem::Initialize()
{
	hitCount_ = 0;
	hitNum_ = 0;
	se_.LoadWave("SE/se.wav");
}

void HitSystem::Update(float time)
{

	std::list<std::unique_ptr<Notes>>& notesList = NotesList::GetInstance()->GetNotesList();

	for (std::unique_ptr<Notes>& notes : notesList) {
		notes->Update(time);

		if (time >= notes->GetTime() + kHitErrorFrame_[GOOD]) {
			notes->Dead();
			score_->AddMiss();

			hitCount_ = 1;
			hitNum_ = MISS;
		}
	}

	HitTest(time);

	for (std::list<std::unique_ptr<Notes>>::iterator i = notesList.begin(); i != notesList.end(); i++) {
		if (i->get()->IsDead()) {
			notesList.erase(i);
			break;
		}
	}
}

void HitSystem::HitTest(float time)
{
	std::list<std::unique_ptr<Notes>>& notesList = NotesList::GetInstance()->GetNotesList();

	Input* input = Input::GetInstance();

	for (std::unique_ptr<Notes>& notes : notesList) {
		if ((input->PressedGamePadButton(Input::GamePadButton::LEFT) && LEFT_ARROW == notes->GetType()) ||
			(input->PressedGamePadButton(Input::GamePadButton::RIGHT) && RIGHT_ARROW == notes->GetType()) ||
			(input->PressedGamePadButton(Input::GamePadButton::UP) && UP_ARROW == notes->GetType()) ||
			(input->PressedGamePadButton(Input::GamePadButton::DOWN) && DOWN_ARROW == notes->GetType())) {
			float errorFrame = std::abs(time - notes->GetTime());

			if (errorFrame <= kHitErrorFrame_[PERFECT]) {
				notes->Dead();


				score_->AddPer();

				hitCount_ = 1;
				hitNum_ = PERFECT;

				se_.Play();
				break;
			}
			else if (errorFrame <= kHitErrorFrame_[GREAT]) {
				notes->Dead();


				score_->AddGre();
				hitCount_ = 1;
				hitNum_ = GREAT;
				se_.Play();
				break;
			}
			else if (errorFrame <= kHitErrorFrame_[GOOD]) {
				notes->Dead();


				score_->AddGood();

				hitCount_ = 1;
				hitNum_ = GOOD;
				se_.Play();
				break;
			}
		}
	}
}
