#pragma once
#include <string>
#include <xaudio2.h>
#include <wrl.h>
#include <vector>
#include <map>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")

using namespace Microsoft::WRL;

class SoundManager
{
	struct SoundData
	{
		WAVEFORMATEX* waveFormatEx;
		std::vector<BYTE> audioData;
	};

private:
	SoundManager();
	~SoundManager();
	SoundManager(const SoundManager& other) = delete;
	void operator=(const SoundManager& other) = delete;

public:
	static SoundManager& Instance();
	bool Init();
	void Update();
	void Shutdown();

	void Load(std::string filepath, std::string name);
	IXAudio2SourceVoice* Play(std::string name, bool isLoop = false);
	IXAudio2SourceVoice* Stop(std::string name);
	IXAudio2MasteringVoice* GetMasterVoice();

private:
	ComPtr<IMFAttributes> sourceReaderConfiguration;
	ComPtr<IXAudio2> xAudio;
	IXAudio2MasteringVoice* masteringVoice;
	std::map<std::string, SoundData> soundDataMap;
	std::vector<IXAudio2SourceVoice*> sourceVoices;
};