#pragma once

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <wrl.h>
#include <string>
#include <vector>
#include <memory>
#include <list>
#include <unordered_map>

class SoundData;

class AudioManager
{
public:

	struct Voice {
		uint32_t handle = 0u;
		IXAudio2SourceVoice* sourceVoice = nullptr;
		const SoundData* soundData = nullptr;
	};

	static AudioManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	void Update();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	const SoundData* LoadWave(const std::string& filename);

	uint32_t Play(const SoundData* soundData, bool loopFlag = false, float volume = 0.7f);

	void Stop(uint32_t voiceHandle, const SoundData* soundData);

	bool IsPlaying(uint32_t voiceHandle, const SoundData* soundData);

	void Pause(uint32_t voiceHandle, const SoundData* soundData);

	void ReStart(uint32_t voiceHandle, const SoundData* soundData);

	void SetVolume(uint32_t voiceHandle, const SoundData* soundData, float volume);

private:
	void Unload(SoundData* soundData);
	void DestroyVoice(uint32_t handle, const SoundData* soundData);
	Voice* FindUnusedVoice();

private:
	AudioManager() = default;
	~AudioManager() = default;
	AudioManager(const AudioManager&) = delete;
	const AudioManager& operator=(const AudioManager&) = delete;

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_ = nullptr;

	std::unordered_map<std::string, std::unique_ptr<SoundData>> soundDataMap_;
	std::list<std::unique_ptr<Voice>> voices_;

	const std::string directoryPath_ = "Resources/Audio/";
};