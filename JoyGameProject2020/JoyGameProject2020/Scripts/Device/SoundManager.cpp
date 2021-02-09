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

	//WMF�̏�����
	result = MFStartup(MF_VERSION);
	if (FAILED(result))
	{
		return false;
	}

	//IMFAttributes�̐���
	result = MFCreateAttributes(sourceReaderConfiguration.ReleaseAndGetAddressOf(), 1);
	if (FAILED(result))
	{
		return false;
	}

	//WMF�ǂݍ��݂��x�����[�h�ɐݒ�
	result = sourceReaderConfiguration->SetUINT32(MF_LOW_LATENCY, true);
	if (FAILED(result))
	{
		return false;
	}

	//XAudio2�f�o�C�X�̐���
#if _DEBUG
	result = XAudio2Create(xAudio.ReleaseAndGetAddressOf(), XAUDIO2_DEBUG_ENGINE);
#else
	result = XAudio2Create(&xAudio);
#endif
	if (FAILED(result))
	{
		return false;
	}

	//�}�X�^�[�{�C�X�̐���
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
	//�X�g���[���C���f�b�N�X
	DWORD streamIndex = (DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM;

	//�\�[�X�ǂݍ��݂̐���
	ComPtr<IMFSourceReader> sourceReader = nullptr;
	result = MFCreateSourceReaderFromURL(
		FileUtility::GetWideStringFromString(filepath).c_str(),
		sourceReaderConfiguration.Get(),
		sourceReader.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return;
	}

	//�S�ẴX�g���[���̑I������
	result = sourceReader->SetStreamSelection((DWORD)MF_SOURCE_READER_ALL_STREAMS, false);
	if (FAILED(result))
	{
		return;
	}

	//�ŏ��̃X�g���[���̑I��
	result = sourceReader->SetStreamSelection(streamIndex, true);
	if (FAILED(result))
	{
		return;
	}

	//���f�B�A�t�@�C���̃N�G�����
	ComPtr<IMFMediaType> nativeMediaType = nullptr;
	result = sourceReader->GetNativeMediaType(streamIndex, 0, nativeMediaType.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return;
	}

	//�{���ɃI�[�f�B�I�t�@�C�����m�F
	GUID majorType{};
	result = nativeMediaType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);
	if (majorType != MFMediaType_Audio)
	{
		return;
	}

	//���k���񈳏k���m�F
	GUID subType{};
	result = nativeMediaType->GetGUID(MF_MT_MAJOR_TYPE, &subType);
	if (subType == MFAudioFormat_Float || subType == MFAudioFormat_PCM)
	{
		//�񈳏k���͉������Ȃ�
	}
	else
	{
		//�𓀏���

		//���f�B�A�^�C�v�̐���
		ComPtr<IMFMediaType>partialType = nullptr;
		result = MFCreateMediaType(partialType.ReleaseAndGetAddressOf());
		if (FAILED(result))
		{
			return;
		}

		//���f�B�A�^�C�v���uaudio�v�ɐݒ�
		result = partialType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		if (FAILED(result))
		{
			return;
		}

		//�񈳏k�f�[�^�̃��N�G�X�g
		result = partialType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
		if (FAILED(result))
		{
			return;
		}

		//���N�G�X�g�̑��M
		result = sourceReader->SetCurrentMediaType(streamIndex, NULL, partialType.Get());
		if (FAILED(result))
		{
			return;
		}
	}

	//�f�[�^���𓀂��AXAudio2�o�b�t�@�ɓǂݍ���
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

	//�X�g���[�����I������Ă��邱�Ƃ̊m�F
	result = sourceReader->SetStreamSelection(streamIndex, true);
	if (FAILED(result))
	{
		return;
	}

	//�f�[�^���R�s�[
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

		//�f�[�^���܂��L�����ǂ����m�F
		if (flags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
		{
			break;
		}

		//�f�[�^���Ōォ�ǂ����m�F
		if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			break;
		}

		if (sample == nullptr)
		{
			continue;
		}

		//�f�[�^��A�������o�b�t�@�ɕϊ�
		result = sample->ConvertToContiguousBuffer(buffer.GetAddressOf());
		if (FAILED(result))
		{
			return;
		}

		//�o�b�t�@�����b�N���A�f�[�^�����[�J���������ɃR�s�[
		result = buffer->Lock(&localAudioData, nullptr, &localAudioDataLength);
		if (FAILED(result))
		{
			return;
		}

		for (auto i = 0; i < localAudioDataLength; ++i)
		{
			audioData.push_back(localAudioData[i]);
		}

		//�o�b�t�@�̃��b�N������
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
