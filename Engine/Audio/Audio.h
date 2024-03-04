#pragma once
#include <stdint.h>
#include <string>

class AudioManager;
class SoundData;
class GlobalVariables;
class VolumeManager;

class Audio
{
public: // gameで使用する関数
	/// <summary>
	/// waveデータのロード,タグの設定,ボリュームの設定(ImGuiで後からいじれるようになってる)
	/// </summary>
	/// <param name="filename">"hoge.wav"で読み込める,Resources/Audio/SE or Musicの中にあれば更に格納しててもok</param>
	/// <param name="itemName">ImGuiに表示する名前,日本語可</param>
	/// <param name="volume">ボリューム</param>
	void LoadWave(const std::string& filename, const std::string& itemName = "_", float volume = 0.7f);
	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="isLoop">ループさせるか</param>
	void Play(bool isLoop = false);
	/// <summary>
	/// 停止
	/// </summary>
	void Stop() const;
	/// <summary>
	/// 再生されているかの取得
	/// </summary>
	/// <returns>再生されているか</returns>
	bool IsPlaying() const;
	/// <summary>
	/// 一時停止
	/// </summary>
	void Pause() const;
	/// <summary>
	/// 一時停止箇所からの再生
	/// </summary>
	void ReStart() const;

public: // エンジン内で使用する関数
	static void StaticInitialize();
	void Update();
	const std::string& GetItemName() const { return itemName_; }
	const SoundData* GetSoundDataPtr() const;
private:
	const SoundData* soundData_;
	uint32_t voiceHandle_;
	float volume_;
	std::string itemName_;

	static AudioManager* audioManager_;
	static VolumeManager* volumeManager_;
	static GlobalVariables* globalVariables_;
};