#pragma once
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <vector>

enum class AudioType;

struct ChunkHeader {
	char id[4];
	int32_t size;
};

struct RiffHeader {
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk {
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

// 音声データ
class SoundData {
public:
	// 波形フォーマット
	WAVEFORMATEX wfex;
	// バッファ
	std::vector<BYTE> pBuffer;
	unsigned int bufferSize;
	AudioType type;
};