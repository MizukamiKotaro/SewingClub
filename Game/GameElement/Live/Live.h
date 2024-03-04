#pragma once
#include <memory>
#include "GameElement/Live/Dome/Dome.h"
#include "GameElement/Live/Screen/Screen.h"
#include "GameElement/Live/Stage/Stage.h"
#include "GameElement/Live/StageLights/StageLights.h"


class Camera;

class Live {
public: 

	Live();

	void Initialize(Camera* camera);

	void Update(float time);

	void Draw(Camera* camera);

	// スクリーンに書き出す
	void WriteScreen(Camera* camera);

private:
	
	std::unique_ptr<Dome> dome_;
	std::unique_ptr<Screen> screen_;
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<StageLights> stageLights_;
};
