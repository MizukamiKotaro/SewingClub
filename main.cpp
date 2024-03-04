#include "Kyoko.h"
#include <Windows.h>
#include <memory>
#include "SceneSystem/SceneManager/SceneManager.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR/* lpCmdLine*/, _In_ int/* nShowCmd*/) {

	Kyoko::Engine::Initialize("LE2A_21_ミズカミ_コタロウ_音ゲー", 1280, 720);

	std::unique_ptr<SceneManager> sceneManager = std::make_unique<SceneManager>();
	sceneManager->Run();

	Kyoko::Engine::Finalize();

	return 0;
}