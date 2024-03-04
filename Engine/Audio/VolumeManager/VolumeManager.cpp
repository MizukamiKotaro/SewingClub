#include "VolumeManager.h"
#include "Audio.h"
#include "AudioConfig.h"
#include "GlobalVariables/GlobalVariables.h"
#include <algorithm>

VolumeManager* VolumeManager::GetInstance()
{
	static VolumeManager instance;
	return &instance;
}

void VolumeManager::Initialize()
{
	seVolume_ = 0.7f;
	musicVolume_ = 0.7f;

	globalVariables_ = GlobalVariables::GetInstance();

	globalVariables_->CreateChunk("Audio");
	globalVariables_->CreateGroup("Audio", "Master");

	globalVariables_->AddItem("Audio", "Master", "SE全体のボリューム", seVolume_);
	globalVariables_->AddItem("Audio", "Master", "Music全体のボリューム", musicVolume_);

	seVolume_ = globalVariables_->GetFloatValue("Audio", "Master", "SE全体のボリューム");
	musicVolume_ = globalVariables_->GetFloatValue("Audio", "Master", "Music全体のボリューム");
}

void VolumeManager::Clear()
{
	audioMap_.clear();
}

void VolumeManager::SetAudio(Audio* audio)
{
	if (audioMap_.find(audio->GetItemName()) == audioMap_.end()) {
		audioMap_[audio->GetItemName()] = std::make_unique<Audio>(*audio);
	}
}

void VolumeManager::Update()
{
#ifdef _DEBUG
	seVolume_ = globalVariables_->GetFloatValue("Audio", "Master", "SE全体のボリューム");
	musicVolume_ = globalVariables_->GetFloatValue("Audio", "Master", "Music全体のボリューム");

	seVolume_ = std::clamp(seVolume_, 0.0f, 1.0f);
	musicVolume_ = std::clamp(musicVolume_, 0.0f, 1.0f);
	for (std::pair<const std::string, std::unique_ptr<Audio>>& audio : audioMap_) {
		audio.second->Update();
	}
#endif // _DEBUG
}