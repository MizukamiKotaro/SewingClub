#pragma once
#include <memory>
#include "GameElement/Live/Live.h"
#include "GameElement/Notes/Notes.h"
#include "GameElement/Score/Score.h"
#include "GameElement/StageUI/StageUI.h"
#include "Camera.h"
#include "Audio.h"
#include "Input.h"

class Game {
public: 

	Game(Camera* camera);

	void Initialize();

	void Update();

	void Draw();

	const bool GetIsClear() const { return isClear_; }

private:

private:
	Input* input_ = nullptr;

	Camera* camera_ = nullptr;

private:
	
	std::unique_ptr<Live> live_;
	std::unique_ptr<Score> score_;
	std::unique_ptr<StageUI> stageUI_;
	NotesList* notesList_ = nullptr;

	std::unique_ptr<Audio> music_;


	float time_;

	bool isStartMusic_;

	float finishCount_;

	bool isMusicFinish_;

	bool isClear_;

	bool isFinish_;

	Audio se_;
};
