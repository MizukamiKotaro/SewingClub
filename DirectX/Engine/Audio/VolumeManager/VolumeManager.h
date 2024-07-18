#pragma once
#include <list>
#include <string>
#include <memory>
#include <unordered_map>

class Audio;
enum class AudioType;
class GlobalVariableUser;

class VolumeManager
{
public:
	static VolumeManager* GetInstance();

	void Initialize();

	void Clear();

	void SetAudio(Audio* audio);

	void Update();

	const float GetSEVolume() const { return seVolume_ * seVolumeStage_; }
	const float GetMusicVolume() const { return musicVolume_ * musicVolumeStage_; }

	void ResetDefalutVolumeStage();
	const float GetSEVolumeStage() const { return seVolumeStage_; }
	const float GetMusicVolumeStage() const { return musicVolumeStage_; }
	void SetSEVolumeStage(const float& volume) { seVolumeStage_ = volume; }
	void SetMusicVolumeStage(const float& volume) { musicVolumeStage_ = volume; }
	void ResetVolumeStage();
	void SaveVolumeStage(const float& seVolumeStage, const float& musicVolumeStage);

private:
	VolumeManager() = default;
	~VolumeManager() = default;
	VolumeManager(const VolumeManager&) = delete;
	const VolumeManager& operator=(const VolumeManager&) = delete;

	std::unordered_map<std::string, std::unique_ptr<Audio>> audioMap_;
	std::unique_ptr<GlobalVariableUser> globalVariables_;
	
	float seVolume_ = 0.7f;
	float musicVolume_ = 0.7f;

	float seVolumeStage_ = 0.7f;
	float musicVolumeStage_ = 0.7f;
};