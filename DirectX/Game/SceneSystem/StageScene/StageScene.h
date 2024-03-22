#pragma once

#include <memory>
#include <array>
#include "SceneSystem/IScene/IScene.h"

#include "GameElement/Player/Player.h"
#include "GameElement/WaveFloor/WaveFloor.h"
#include "InstancingModelManager.h"
#include "GameElement/Wave/Wave.h"
#include "GameElement/WaterChunk/WaterChunk.h"

class StageScene : public IScene
{
public:

	StageScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

	void WaveUpdate();

private:

	std::unique_ptr<Player> player_;
	std::unique_ptr<WaveFloor> waveFloor_;
	std::list<std::unique_ptr<Wave>> waves_;
	std::array<std::unique_ptr<WaterChunk>,3> water_;

	float time_ = 0.0f;

	InstancingModelManager* instancingmodelManager_;
};
