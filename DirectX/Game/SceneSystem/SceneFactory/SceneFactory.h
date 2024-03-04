#pragma once

#include "SceneSystem/ISceneFactory/ISceneFactory.h"

class SceneFactory : public ISceneFactory
{
public:
	IScene* CreateScene(int sceneNo) override;
};
