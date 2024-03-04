#include "SceneSystem/SceneFactory/SceneFactory.h"

#include "SceneSystem/TitleScene/TitleScene.h"
#include "SceneSystem/StageScene/StageScene.h"
#include "SceneSystem/SelectScene/SelectScene.h"
#include "SceneSystem/ClearScene/ClearScene.h"

IScene* SceneFactory::CreateScene(int sceneNo)
{
	IScene* newScene = nullptr;

	if (sceneNo == TITLE) {
		newScene = new TitleScene();
	}
	else if (sceneNo == SELECT) {
		newScene = new SelectScene();
	}
	else if (sceneNo == STAGE) {
		newScene = new StageScene();
	}
	else if (sceneNo == CLEAR) {
		newScene = new ClearScene();
	}

	return newScene;
}