#pragma once

#include <memory>
#include "SceneSystem/IScene/IScene.h"
#include "Sprite.h"
#include "Contrast/Contrast.h"
#include "Light/Light.h"
#include "HighLumi/HighLumi.h"
#include "Blur/Blur.h"
#include "GaussianBlur/GaussianBlur.h"
#include "Bloom/Bloom.h"
#include "PostEffect/PostEffect.h"
#include "GameElement/Game.h"

class StageScene : public IScene
{
public:

	StageScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

	std::unique_ptr<Game> game_;
};
