#include "Game.h"
#include "Camera.h"
#include "FrameInfo/FrameInfo.h"
#include "GameElement/HitSystem/HitSystem.h"
#include "GameElement/ScoreManager/ScoreManager.h"

Game::Game(Camera* camera)
{
	input_ = Input::GetInstance();

	camera_ = camera;
	camera_->Initialize();

	finishCount_ = 0.0f;
	isMusicFinish_ = false;
	isStartMusic_ = false;
	isClear_ = false;

	live_ = std::make_unique<Live>();

	music_ = std::make_unique<Audio>();
	music_->LoadWave("Music/maou_short_14_shining_star.wav");
	
	notesList_ = NotesList::GetInstance();
	notesList_->Initialize();

	score_ = std::make_unique<Score>();

	stageUI_ = std::make_unique<StageUI>();
	stageUI_->SetIsMusicFinish(&isMusicFinish_);
	stageUI_->SetScore(score_.get());

	HitSystem::SetScore(score_.get());

	se_.LoadWave("SE/finish.wav");
}

void Game::Initialize()
{
	live_->Initialize(camera_);

	time_ = 0.0f;

	notesList_->PopCommands();

	score_->Reset();
	finishCount_ = 0.0f;
	isMusicFinish_ = false;
	isStartMusic_ = false;
	isClear_ = false;

	camera_->Initialize();

	stageUI_->Initialize();

	HitSystem::Initialize();

	isFinish_ = false;
}

void Game::Update()
{
	if (!isStartMusic_) {
		isStartMusic_ = true;
		music_->Play();
	}

	if (!music_->IsPlaying()) {
		isMusicFinish_ = true;
	}

	time_ += FrameInfo::GetInstance()->GetDeltaTime();

	live_->Update(time_);

	HitSystem::Update(time_);

	if (isMusicFinish_) {
		finishCount_ += FrameInfo::GetInstance()->GetDeltaTime();

		if (!isFinish_) {
			isFinish_ = true;
			se_.Play();
		}

		if (finishCount_ >= 0.75f) {
			isClear_ = true;
			ScoreManager::GetInstance()->SetClearScore(score_.get());
		}
	}
}

void Game::Draw()
{
	live_->Draw(camera_);

	notesList_->Draw(camera_);

	stageUI_->Draw();
}


