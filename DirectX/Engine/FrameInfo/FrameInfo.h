#pragma once
#include <chrono>

class FrameInfo
{
public:
	static FrameInfo* GetInstance();

	void Initialize();

	void End();

	float GetDeltaTime() const { return deltaTime_; }

	float GetFramerate() const { return 1.0f / deltaTime_; }

private:
	FrameInfo() = default;
	~FrameInfo() = default;
	FrameInfo(const FrameInfo&) = delete;
	const FrameInfo& operator=(const FrameInfo&) = delete;

private:

	std::chrono::steady_clock::time_point frameStartTime_;
	float deltaTime_;
};