#include "SoundManager.h"
#include "Utility/FileUtility.h"

SoundManager::SoundManager() :
	sourceReaderConfiguration(nullptr),
	xAudio(nullptr),
	masteringVoice(nullptr)
{
}

SoundManager::~SoundManager()
{
}

SoundManager& SoundManager::Instance()
{
	static SoundManager instance;
	return instance;
}

bool SoundManager::Init()
{
	HRESULT result;

	//WMFの初期化
	result = MFStartup(MF_VERSION);
	if (FAILED(result))
	{
		return false;
	}

	//IMFAttributesの生成
	result = MFCreateAttributes(sourceReaderConfiguration.ReleaseAndGetAddressOf(), 1);
	if (FAILED(result))
	{
		return false;
	}

	//WMF読み込みを低遅延モードに設定
	result = sourceReaderConfiguration->SetUINT32(MF_LOW_LATENCY, true);
	if (FAILED(result))
	{
		return false;
	}

	//XAudio2デバイスの生成
#if _DEBUG
	result = XAudio2Create(xAudio.ReleaseAndGetAddressOf(), XAUDIO2_DEBUG_ENGINE);
#else
	result = XAudio2Create(&xAudio);
#endif
	if (FAILED(result))
	{
		return false;
	}

	//マスターボイスの生成
	result = xAudio->CreateMasteringVoice(&masteringVoice);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void SoundManager::Update()
{
	std::vector<int> itrs{};

	for (int i = 0; i < sourceVoices.size(); ++i)
	{
		XAUDIO2_VOICE_STATE state;
		sourceVoices[i]->GetState(&state);
		bool isRunning = state.BuffersQueued > 0;
		if (!isRunning)
			itrs.push_back(i);
	}

	for (int i = itrs.size() - 1; i >= 0; --i)
	{
		sourceVoices.erase(sourceVoices.begin() + itrs[i]);
	}
}

void SoundManager::Shutdown()
{
	for (auto& voice : sourceVoices)
	{
		voice->DestroyVoice();
		voice = nullptr;
	}

	if (masteringVoice)
	{
		masteringVoice->DestroyVoice();
		masteringVoice = nullptr;
	}
}

void SoundManager::Load(std::string filepath, std::string name)
{
	HRESULT result;
	//ストリームインデックス
	DWORD streamIndex = (DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM;

	//ソース読み込みの生成
	ComPtr<IMFSourceReader> sourceReader = nullptr;
	result = MFCreateSourceReaderFromURL(
		FileUtility::GetWideStringFromString(filepath).c_str(),
		sourceReaderConfiguration.Get(),
		sourceReader.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return;
	}

	//全てのストリームの選択解除
	result = sourceReader->SetStreamSelection((DWORD)MF_SOURCE_READER_ALL_STREAMS, false);
	if (FAILED(result))
	{
		return;
	}

	//最初のストリームの選択
	result = sourceReader->SetStreamSelection(streamIndex, true);
	if (FAILED(result))
	{
		return;
	}

	//メディアファイルのクエリ情報
	ComPtr<IMFMediaType> nativeMediaType = nullptr;
	result = sourceReader->GetNativeMediaType(streamIndex, 0, nativeMediaType.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return;
	}

	//本当にオーディオファイルか確認
	GUID majorType{};
	result = nativeMediaType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);
	if (majorType != MFMediaType_Audio)
	{
		return;
	}

	//圧縮か非圧縮か確認
	GUID subType{};
	result = nativeMediaType->GetGUID(MF_MT_MAJOR_TYPE, &subType);
	if (subType == MFAudioFormat_Float || subType == MFAudioFormat_PCM)
	{
		//非圧縮時は何もしない
	}
	else
	{
		//解凍処理

		//メディアタイプの生成
		ComPtr<IMFMediaType>partialType = nullptr;
		result = MFCreateMediaType(partialType.ReleaseAndGetAddressOf());
		if (FAILED(result))
		{
			return;
		}

		//メディアタイプを「audio」に設定
		result = partialType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		if (FAILED(result))
		{
			return;
		}

		//非圧縮データのリクエスト
		result = partialType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
		if (FAILED(result))
		{
			return;
		}

		//リクエストの送信
		result = sourceReader->SetCurrentMediaType(streamIndex, NULL, partialType.Get());
		if (FAILED(result))
		{
			return;
		}
	}

	//データを解凍し、XAudio2バッファに読み込み
	ComPtr<IMFMediaType>uncompressedAudioType = nullptr;
	result = sourceReader->GetCurrentMediaType(streamIndex, uncompressedAudioType.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return;
	}

	unsigned int waveLength;
	result = MFCreateWaveFormatExFromMFMediaType(uncompressedAudioType.Get(), &soundDataMap[name].waveFormatEx, &waveLength);
	if (FAILED(result))
	{
		return;
	}

	//ストリームが選択されていることの確認
	result = sourceReader->SetStreamSelection(streamIndex, true);
	if (FAILED(result))
	{
		return;
	}

	//データをコピー
	ComPtr<IMFSample> sample = nullptr;
	ComPtr<IMFMediaBuffer>buffer = nullptr;
	BYTE* localAudioData = NULL;
	DWORD localAudioDataLength = 0;
	std::vector<BYTE> audioData;

	while (true)
	{
		DWORD flags = 0;
		result = sourceReader->ReadSample(streamIndex, 0, nullptr, &flags, nullptr, sample.ReleaseAndGetAddressOf());
		if (FAILED(result))
		{
			return;
		}

		//データがまだ有効かどうか確認
		if (flags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
		{
			break;
		}

		//データが最後かどうか確認
		if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			break;
		}

		if (sample == nullptr)
		{
			continue;
		}

		//データを連続したバッファに変換
		result = sample->ConvertToContiguousBuffer(buffer.GetAddressOf());
		if (FAILED(result))
		{
			return;
		}

		//バッファをロックし、データをローカルメモリにコピー
		result = buffer->Lock(&localAudioData, nullptr, &localAudioDataLength);
		if (FAILED(result))
		{
			return;
		}

		for (auto i = 0; i < localAudioDataLength; ++i)
		{
			audioData.push_back(localAudioData[i]);
		}

		//バッファのロックを解除
		result = buffer->Unlock();
		localAudioData = nullptr;
		if (FAILED(result))
		{
			return;
		}
	}

	soundDataMap[name].audioData = audioData;
}

IXAudio2SourceVoice* SoundManager::Play(std::string name, bool isLoop)
{
	if (!soundDataMap.count(name)) {
		return nullptr;
	}

	HRESULT result;

	IXAudio2SourceVoice* sourceVoice;
	result = xAudio->CreateSourceVoice(&sourceVoice, soundDataMap[name].waveFormatEx);
	if (FAILED(result))
		return nullptr;

	XAUDIO2_BUFFER audioBuffer;
	ZeroMemory(&audioBuffer, sizeof(XAUDIO2_BUFFER));
	audioBuffer.AudioBytes = (UINT32)soundDataMap[name].audioData.size();
	audioBuffer.pAudioData = (BYTE* const)&soundDataMap[name].audioData[0];
	audioBuffer.pContext = nullptr;
	if (isLoop)
		audioBuffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	result = sourceVoice->SubmitSourceBuffer(&audioBuffer);
	if (FAILED(result))
		return nullptr;
	sourceVoices.push_back(sourceVoice);

	sourceVoice->Start();

	return sourceVoice;
}

IXAudio2SourceVoice* SoundManager::Stop(std::string name)
{
	HRESULT result;

	IXAudio2SourceVoice* sourceVoice;
	result = xAudio->CreateSourceVoice(&sourceVoice, soundDataMap[name].waveFormatEx);
	if (FAILED(result))
		return nullptr;

	XAUDIO2_BUFFER audioBuffer;
	ZeroMemory(&audioBuffer, sizeof(XAUDIO2_BUFFER));
	audioBuffer.AudioBytes = (UINT32)soundDataMap[name].audioData.size();
	audioBuffer.pAudioData = (BYTE* const)&soundDataMap[name].audioData[0];
	audioBuffer.pContext = nullptr;

	result = sourceVoice->SubmitSourceBuffer(&audioBuffer);
	if (FAILED(result))
		return nullptr;

	auto itr = std::find(sourceVoices.begin(), sourceVoices.end(), sourceVoice);
	if (itr != sourceVoices.end()) {
		std::iter_swap(itr, sourceVoices.end() - 1);
		sourceVoices.pop_back();
	}

	sourceVoice->Stop();

	return sourceVoice;
}

IXAudio2MasteringVoice* SoundManager::GetMasterVoice()
{
	return masteringVoice;
}
