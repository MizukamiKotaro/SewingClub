#include "Kyoko.h"
#include "Engine/Base/WindowsInfo/WindowsInfo.h"
#include "Engine/Base/ResourceLeakChecker.h"
#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Input/Input.h"
#include "TextureManager/TextureManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include "GlobalVariables/GlobalVariables.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "ModelDataManager.h"
#include "Utils/RandomGenerator/RandomGenerator.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "AudioManager/AudioManager.h"
#include "FrameInfo/FrameInfo.h"
#include "Sprite.h"
#include "Model.h"
#include "BasePostEffect/BasePostEffect.h"
#include "LightSingleton/LightSingleton.h"
#include "Drawers/IDrawer/IDrawer.h"
#include "VolumeManager/VolumeManager.h"
#include "Audio.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include "ParticleManager.h"

static ResourceLeackChecker leakCheck;

DirectXBase* dxBase;
WindowsInfo* winInfo;
GraphicsPipelineManager* gpoManager;
AudioManager* audioManager;
Input* inputManager;
GlobalVariables* globalVariables;
VolumeManager* volumeManager;

void Kyoko::Engine::Initialize(const char* windowName, int width, int height)
{
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	SUCCEEDED(hr);
	winInfo = WindowsInfo::GetInstance();
	winInfo->CreateGameWindow(windowName, width, height);

	dxBase = DirectXBase::GetInstance();
	dxBase->Initialize();

	Kyoko::ImGuiManager::Initialize();

	TextureManager::GetInstance()->Initialize();

	gpoManager = GraphicsPipelineManager::GetInstance();
	gpoManager->Initialize();

	globalVariables = GlobalVariables::GetInstance();
	globalVariables->Initialize();
	globalVariables->LoadFiles();

	GlobalVariableUser::StaticInitialize();

	IDrawer::StaticInitialize();
	Sprite::StaticInitialize();
	Model::StaticInitialize();
	BasePostEffect::StaticInitialize();
	ParticleManager::GetInstance()->FirstInitialize();

	audioManager = AudioManager::GetInstance();
	audioManager->Initialize();
	volumeManager = VolumeManager::GetInstance();
	volumeManager->Initialize();
	Audio::StaticInitialize();

	inputManager = Input::GetInstance();
	inputManager->Initialize();

	LightSingleton::GetInstance()->Initialize();

	FrameInfo::GetInstance()->Initialize();

#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(dxBase->GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		//ヤバいエラーの時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラーのときに止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		//抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] = {
			//windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーバグ
			//https:://stackoverflow.com/question/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);

		//解放
		infoQueue->Release();
	}
#endif // _DEBUG
}

const bool Kyoko::Engine::ProcessMessage()
{
	return winInfo->ProcessMessage();
}

void Kyoko::Engine::FirstUpdateInLoop()
{
	ImGuiManager::Begin();
	inputManager->Update();
	audioManager->Update();
#ifdef _DEBUG
	volumeManager->Update();
	globalVariables->Update();
#endif // _DEBUG
}

void Kyoko::Engine::PreDraw()
{
	ImGuiManager::End();

	// 描画前処理
	dxBase->PreDraw();

	gpoManager->PreDraw();
}

void Kyoko::Engine::PostDraw()
{
	ImGuiManager::Draw();

	// 描画後処理
	dxBase->PostDraw();
}

void Kyoko::Engine::Finalize()
{
#pragma region 基盤システムの終了
	ImGuiManager::Finalize();

	CoUninitialize();
	audioManager->Finalize();
	TextureManager::GetInstance()->Finalize();
	ModelDataManager::GetInstance()->Finalize();
	DescriptorHeapManager::GetInstance()->Finalize();
	dxBase->Finalize();
	winInfo->Finalize();

#pragma endregion 基盤システムの終了
}