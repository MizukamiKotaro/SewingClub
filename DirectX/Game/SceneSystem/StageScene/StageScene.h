#pragma once

#include <memory>
#include "SceneSystem/IScene/IScene.h"

#include "GameElement/Player/Player.h"
#include "InstancingModelManager.h"

class StageScene : public IScene
{
public:

	StageScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

	std::unique_ptr<Player> player_;

	InstancingModelManager* instancingmodelManager_;
};
