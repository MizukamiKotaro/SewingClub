#pragma once

#include <memory>
#include "SceneSystem/IScene/IScene.h"
#include "SceneSystem/ISceneFactory/ISceneFactory.h"

class Input;
class FrameInfo;

class SceneManager
{
private:

	//デバッグウィンドウ
	void DebugWindow();

private:
	// シーンを保持するメンバ変数
	std::unique_ptr<IScene> scene_;

	int currentSceneNo_; // 現在のシーン
	int preSceneNo_; // 前のシーン

	std::unique_ptr<ISceneFactory> sceneFactory_;

	Input* inputManager_;
	FrameInfo* frameInfo_;

	//デバッグ用シーン名
	std::vector<std::string> sceneName_;
public:
	SceneManager();
	int Run();
};