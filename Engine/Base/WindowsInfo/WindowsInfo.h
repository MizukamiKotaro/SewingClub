#pragma once

#include <Windows.h>
#include <cstdint>
#include <string>
#include "Vector2.h"

class WindowsInfo
{
public: // 静的メンバ関数
	static WindowsInfo* GetInstance();

	//ウィンドウプロシャープ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // メンバ関数

	void CreateGameWindow(const std::string& windowName = "LE2A_21_ミズカミ_コタロウ", int width = 1280, int height = 720);

	// メッセージ処理
	bool ProcessMessage();

	void Finalize();

	HWND GetHwnd() const { return hwnd_; }

	HINSTANCE GetHInstance() const { return wndClass_.hInstance; }

	const Vector2 GetWindowSize() const { return windowSize_; }

private: // メンバ関数
	WindowsInfo() = default;
	~WindowsInfo() = default;
	WindowsInfo(const WindowsInfo&) = delete;
	const WindowsInfo& operator=(const WindowsInfo&) = delete;

private: // メンバ変数
	// Window関連
	HWND hwnd_ = nullptr;   // ウィンドウハンドル
	WNDCLASS wndClass_{}; // ウィンドウクラス
	Vector2 windowSize_;
};