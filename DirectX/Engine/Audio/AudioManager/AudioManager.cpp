#include "AudioManager.h"
#include <cassert>
#include <fstream>
#include <filesystem>
#include "Audio.h"
#include "SoundData.h"
#include "AudioConfig.h"
#include "VolumeManager/VolumeManager.h"
#include "DebugLog/DebugLog.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

AudioManager* AudioManager::GetInstance() {
	static AudioManager instance;
	return &instance;
}

void AudioManager::Initialize() {


	HRESULT hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(hr));
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(hr));

	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));
	MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
}

void AudioManager::Update()
{
	for (const std::unique_ptr<Voice>& voice : voices_) {
		if (!IsPlaying(voice->handle, voice->soundData)) {
			if (voice->sourceVoice) {
				voice->sourceVoice->DestroyVoice();
				voice->sourceVoice = nullptr;
			}
		}
	}
}

void AudioManager::Finalize() {

	xAudio2_.Reset();

	for (const std::pair<const std::string, std::unique_ptr<SoundData>>& pair : soundDataMap_) {
		Unload(pair.second.get());
	}

	MFShutdown();

	CoUninitialize();
}

const SoundData* AudioManager::LoadWave(const std::string& filename) {

	std::filesystem::path filePathName(filename);
	std::string fileName = filePathName.filename().string();

	if (soundDataMap_.find(fileName) != soundDataMap_.end()) {
		return soundDataMap_[fileName].get();
	}

	SoundData soundData = {};

	std::string entryPath;

	std::filesystem::path dir(directoryPath_);
	bool found = false;
	for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(dir)) {
		if (entry.is_regular_file() && entry.path().filename().string() == fileName) {
			entryPath = entry.path().string();
			found = true;

			for (const std::filesystem::path& component : entry.path()) {
				if (component.filename() == "SE") {
					soundData.type = AudioType::SE;
					break;
				}
				else if (component.filename() == "Music") {
					soundData.type = AudioType::MUSIC;
					break;
				}
			}
			break;
		}
	}

	if (!found) {
		
	}

	std::ifstream file;

	file.open(entryPath, std::ios_base::binary);

	assert(file.is_open());

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	FormatChunk fmt = {};

	file.read((char*)&fmt.chunk, sizeof(ChunkHeader));
	while (strncmp(fmt.chunk.id, "fmt ", 4) != 0) {
		// 読み取りチャンクを検出した場合
		file.seekg(fmt.chunk.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&fmt.chunk, sizeof(ChunkHeader));
	}

	std::vector<char> fmtData(fmt.chunk.size);
	file.read(fmtData.data(), fmt.chunk.size);
	memcpy(&fmt.fmt, fmtData.data(), sizeof(fmt.fmt));

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	while (strncmp(data.id, "data", 4) != 0) {
		// 読み取りチャンクを検出した場合
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	std::vector<BYTE> pBuffer(data.size);
	file.read(reinterpret_cast<char*>(pBuffer.data()), data.size);

	file.close();

	soundData.wfex = fmt.fmt;
	soundData.pBuffer = pBuffer;
	soundData.bufferSize = data.size;

	soundDataMap_[fileName] = std::make_unique<SoundData>(soundData);

	return soundDataMap_[fileName].get();
}

const SoundData* AudioManager::LoadMP3(const std::string& filename)
{
	std::filesystem::path filePathName(filename);
	std::string fileName = filePathName.filename().string();

	if (soundDataMap_.find(fileName) != soundDataMap_.end()) {
		return soundDataMap_[fileName].get();
	}

	SoundData soundData = {};

	std::string entryPath;

	std::filesystem::path dir(directoryPath_);
	bool found = false;
	for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(dir)) {
		if (entry.is_regular_file() && entry.path().filename().string() == fileName) {
			entryPath = entry.path().string();
			found = true;

			for (const std::filesystem::path& component : entry.path()) {
				if (component.filename() == "SE") {
					soundData.type = AudioType::SE;
					break;
				}
				else if (component.filename() == "Music") {
					soundData.type = AudioType::MUSIC;
					break;
				}
			}
			break;
		}
	}

	std::wstring path = DebugLog::ConvertString(entryPath);
	IMFSourceReader* pMFSourceReader{ nullptr };
	MFCreateSourceReaderFromURL(path.c_str(), nullptr, &pMFSourceReader);

	IMFMediaType* pMFMediaType{ nullptr };
	MFCreateMediaType(&pMFMediaType);
	pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	pMFSourceReader->SetCurrentMediaType(static_cast<UINT32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), nullptr, pMFMediaType);

	pMFMediaType->Release();
	pMFMediaType = nullptr;
	pMFSourceReader->GetCurrentMediaType(static_cast<UINT32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &pMFMediaType);

	WAVEFORMATEX* waveFormat{ nullptr };
	MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &waveFormat, nullptr);

	std::vector<BYTE> mediaData;
	while (true)
	{
		IMFSample* pMFSample{ nullptr };
		DWORD dwStreamFlags{ 0 };
		pMFSourceReader->ReadSample(static_cast<UINT32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			break;
		}

		IMFMediaBuffer* pMFMediaBuffer{ nullptr };
		pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

		BYTE* pBuffer{ nullptr };
		DWORD cbCurrentLength{ 0 };
		pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

		mediaData.resize(mediaData.size() + cbCurrentLength);
		memcpy(mediaData.data() + mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

		pMFMediaBuffer->Unlock();

		pMFMediaBuffer->Release();
		pMFSample->Release();
	}

	soundData.wfex = *waveFormat;
	soundData.pBuffer = mediaData;
	soundData.bufferSize = sizeof(BYTE) * static_cast<UINT32>(mediaData.size());

	soundDataMap_[fileName] = std::make_unique<SoundData>(soundData);

	CoTaskMemFree(waveFormat);
	pMFMediaType->Release();
	pMFSourceReader->Release();

	return soundDataMap_[fileName].get();
}

void AudioManager::Unload(SoundData* soundData) {

	soundData->pBuffer.clear();
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void AudioManager::DestroyVoice(uint32_t handle, const SoundData* soundData)
{
	for (const std::unique_ptr<Voice>& voice : voices_) {
		if (voice->handle == handle && voice->soundData == soundData) {
			if (voice->sourceVoice) {
				voice->sourceVoice->DestroyVoice();
				voice->sourceVoice = nullptr;
			}
			break;
		}
	}
}

void AudioManager::DestroyVoiceSameSound(const SoundData* soundData)
{
	for (const std::unique_ptr<Voice>& voice : voices_) {
		if (voice->soundData == soundData) {
			if (voice->sourceVoice) {
				voice->sourceVoice->DestroyVoice();
				voice->sourceVoice = nullptr;
			}
		}
	}
}

AudioManager::Voice* AudioManager::FindUnusedVoice()
{
	for (const std::unique_ptr<Voice>& voice : voices_) {
		if (!voice->sourceVoice) {
			return voice.get();
		}
	}
	Voice voice;
	voice.sourceVoice = nullptr;
	voice.handle = static_cast<uint32_t>(voices_.size());
	voices_.push_back(std::make_unique<Voice>(voice));

	return voices_.back().get();
}

uint32_t AudioManager::Play(const SoundData* soundData, bool loopFlag, float volume) {

	HRESULT hr;

	Voice* voice = FindUnusedVoice();

	hr = xAudio2_->CreateSourceVoice(&voice->sourceVoice, &soundData->wfex);
	assert(SUCCEEDED(hr));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData->pBuffer.data();
	buf.AudioBytes = soundData->bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	if (loopFlag) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	hr = voice->sourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(hr));
	hr = voice->sourceVoice->Start();
	assert(SUCCEEDED(hr));
	hr = voice->sourceVoice->SetVolume(volume);

	voice->soundData = soundData;

	return voice->handle;
}

void AudioManager::Stop(uint32_t voiceHandle, const SoundData* soundData) {

	DestroyVoice(voiceHandle, soundData);
}

bool AudioManager::IsPlaying(uint32_t voiceHandle, const SoundData* soundData)
{
	for (const std::unique_ptr<Voice>& voice : voices_) {
		if (voice->handle == voiceHandle && voice->soundData == soundData) {
			if (voice->sourceVoice) {
				XAUDIO2_VOICE_STATE state{};
				voice->sourceVoice->GetState(&state);
				if (state.BuffersQueued != 0) {
					return true;
				}
				return false;
			}
		}
	}

	return false;
}

void AudioManager::Pause(uint32_t voiceHandle, const SoundData* soundData)
{
	for (const std::unique_ptr<Voice>& voice : voices_) {
		if (voice->handle == voiceHandle && voice->soundData == soundData) {
			if (voice->sourceVoice) {
				voice->sourceVoice->Stop();
				break;
			}
		}
	}
}

void AudioManager::ReStart(uint32_t voiceHandle, const SoundData* soundData)
{
	for (const std::unique_ptr<Voice>& voice : voices_) {
		if (voice->handle == voiceHandle && voice->soundData == soundData) {
			if (voice->sourceVoice) {
				voice->sourceVoice->Start();
				break;
			}
		}
	}
}

void AudioManager::SetVolume(uint32_t voiceHandle, const SoundData* soundData, float volume) {
	for (const std::unique_ptr<Voice>& voice : voices_) {
		if (voice->handle == voiceHandle && voice->soundData == soundData) {
			voice->sourceVoice->SetVolume(volume);
			break;
		}
	}
}

void AudioManager::AllStop()
{
	for (const std::unique_ptr<Voice>& voice : voices_) {
		if (voice->sourceVoice) {
			voice->sourceVoice->DestroyVoice();
			voice->sourceVoice = nullptr;
		}
	}
}

void AudioManager::StopSameSounds(const SoundData* soundData)
{
	DestroyVoiceSameSound(soundData);
}

bool AudioManager::IsPlayingSameSound(const SoundData* soundData)
{
	for (const std::unique_ptr<Voice>& voice : voices_) {
		if (voice->soundData == soundData) {
			if (voice->sourceVoice) {
				XAUDIO2_VOICE_STATE state{};
				voice->sourceVoice->GetState(&state);
				if (state.BuffersQueued != 0) {
					return true;
				}
				return false;
			}
		}
	}

	return false;
}

void AudioManager::SetVolumeSameSound(const SoundData* soundData, float volume)
{
	for (const std::unique_ptr<Voice>& voice : voices_) {
		if (voice->soundData == soundData) {
			voice->sourceVoice->SetVolume(volume);
		}
	}
}
