#include "Audio.h"
#include "AudioManager/AudioManager.h"
#include "VolumeManager/VolumeManager.h"
#include "GlobalVariables/GlobalVariables.h"
#include "AudioConfig.h"
#include "SoundData.h"
#include <algorithm>
#include "ImGuiManager/ImGuiManager.h"

AudioManager* Audio::audioManager_ = nullptr;
VolumeManager* Audio::volumeManager_ = nullptr;
GlobalVariables* Audio::globalVariables_ = nullptr;

void Audio::StaticInitialize()
{
	audioManager_ = AudioManager::GetInstance();
	volumeManager_ = VolumeManager::GetInstance();
	globalVariables_ = GlobalVariables::GetInstance();

	globalVariables_->CreateChunk("Audio");
	globalVariables_->CreateGroup("Audio", "SE");
	globalVariables_->CreateGroup("Audio", "Music");
}

void Audio::LoadWave(const std::string& filename, const std::string& itemName, float volume)
{
	soundData_ = audioManager_->LoadWave(filename);
	volume_ = volume;

	if (itemName == "_") {
		itemName_ = filename;
	}
	else {
		itemName_ = itemName;
	}

	if (soundData_->type == AudioType::SE) {
		globalVariables_->AddItem("Audio", "SE", itemName_ + "のボリューム", volume_);
		volume_ = globalVariables_->GetFloatValue("Audio", "SE", itemName_ + "のボリューム");
	}
	else if (soundData_->type == AudioType::MUSIC) {
		globalVariables_->AddItem("Audio", "Music", itemName_ + "のボリューム", volume_);
		volume_ = globalVariables_->GetFloatValue("Audio", "Music", itemName_ + "のボリューム");
	}
#ifdef _DEBUG
	volumeManager_->SetAudio(this);
#endif // _DEBUG
}

void Audio::Play(bool isLoop)
{
#ifdef _DEBUG
	if (soundData_->type == AudioType::SE) {
		volume_ = globalVariables_->GetFloatValue("Audio", "SE", itemName_ + "のボリューム");
	}
	else if (soundData_->type == AudioType::MUSIC) {
		volume_ = globalVariables_->GetFloatValue("Audio", "Music", itemName_ + "のボリューム");
	}

	volume_ = std::clamp(volume_, 0.0f, 1.0f);
#endif // _DEBUG

	float volume = volume_;
	if (soundData_->type == AudioType::SE) {
		volume *= volumeManager_->GetSEVolume();
	}
	else if (soundData_->type == AudioType::MUSIC) {
		volume *= volumeManager_->GetMusicVolume();
	}
	voiceHandle_ = audioManager_->Play(soundData_, isLoop, volume);
}

void Audio::Stop() const
{
	audioManager_->Stop(voiceHandle_, soundData_);
}

bool Audio::IsPlaying() const
{
	return audioManager_->IsPlaying(voiceHandle_, soundData_);
}

void Audio::Pause() const
{
	audioManager_->Pause(voiceHandle_, soundData_);
}

void Audio::ReStart() const
{
	audioManager_->ReStart(voiceHandle_, soundData_);
}

void Audio::Update()
{
#ifdef _DEBUG
	if (soundData_->type == AudioType::SE) {
		volume_ = globalVariables_->GetFloatValue("Audio", "SE", itemName_ + "のボリューム");
}
	else if (soundData_->type == AudioType::MUSIC) {
		volume_ = globalVariables_->GetFloatValue("Audio", "Music", itemName_ + "のボリューム");
	}

	volume_ = std::clamp(volume_, 0.0f, 1.0f);

	if (!ImGui::Begin("Audio", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
	}
	else {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("音量再生")) {
				std::string playButtonName = itemName_ + "の再生";
				if (ImGui::Button(playButtonName.c_str())) {
					if (IsPlaying()) {
						Stop();
					}
					Play();
				}
				std::string stopButtonName = itemName_ + "の停止";
				if (ImGui::Button(stopButtonName.c_str())) {
					if (IsPlaying()) {
						Stop();
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	if (IsPlaying()) {
		float volume = volume_;
		if (soundData_->type == AudioType::SE) {
			volume *= volumeManager_->GetSEVolume();
		}
		else if (soundData_->type == AudioType::MUSIC) {
			volume *= volumeManager_->GetMusicVolume();
		}

		audioManager_->SetVolume(voiceHandle_, soundData_, volume);
	}
#endif // _DEBUG
}

const SoundData* Audio::GetSoundDataPtr() const
{
	return soundData_;
}
