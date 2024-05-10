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

	globalVariables_->CreateGroup("Audio", "Master");

	globalVariables_->AddItem("Audio", "Master", "SE全体のボリューム", seVolume_);
	globalVariables_->AddItem("Audio", "Master", "Music全体のボリューム", musicVolume_);

	seVolume_ = globalVariables_->GetFloatValue("Audio", "Master", "SE全体のボリューム");
	musicVolume_ = globalVariables_->GetFloatValue("Audio", "Master", "Music全体のボリューム");

	globalVariables_->AddItemDontTouchImGui("SE全体のプレイヤー設定", seVolumeStage_);
	globalVariables_->AddItemDontTouchImGui("Music全体のプレイヤー設定", musicVolumeStage_);

	ResetVolumeStage();
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

void VolumeManager::ResetDefalutVolumeStage()
{
	seVolumeStage_ = 0.7f;
	musicVolumeStage_ = 0.7f;
}

void VolumeManager::ResetVolumeStage()
{
	seVolumeStage_ = globalVariables_->GetFloatValueDontTouchImGui("SE全体のプレイヤー設定");
	musicVolumeStage_ = globalVariables_->GetFloatValueDontTouchImGui("Music全体のプレイヤー設定");
}

void VolumeManager::SaveVolumeStage(const float& seVolumeStage, const float& musicVolumeStage)
{
	seVolumeStage_ = seVolumeStage;
	musicVolumeStage_ = musicVolumeStage;
	globalVariables_->SaveAndSetVariableDontTouchImGui("SE全体のプレイヤー設定", seVolumeStage_);
	globalVariables_->SaveAndSetVariableDontTouchImGui("Music全体のプレイヤー設定", musicVolumeStage_);
}
