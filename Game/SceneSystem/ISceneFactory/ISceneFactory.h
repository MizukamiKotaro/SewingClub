#pragma once

class IScene;

class ISceneFactory
{
public:
	virtual ~ISceneFactory() = default;

	virtual IScene* CreateScene(int sceneNo) = 0;
};