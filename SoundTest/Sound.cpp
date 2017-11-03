#include "pch.h"
#include "Sound.h"

using namespace DirectX;

Sound::Sound() :
	m_nowPlayingKey()
{
}

void Sound::Initialize()
{
	Sound& ins = Get();

	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	ins.m_audEngine = std::make_unique<AudioEngine>(eflags);

	Load();
}

void Sound::Load()
{
	Sound& ins = Get();
	ins.LoadSE(L"Sound.wav");
	ins.LoadBGM(L"Bgm01.wav");
	ins.LoadBGM(L"Bgm02.wav");
}

void Sound::LoadSE(const std::wstring& filename)
{
	Sound& ins = Get();
	ins.m_seDict.emplace(filename, std::make_unique<SoundEffect>(ins.m_audEngine.get(), filename.c_str()));
}

void Sound::LoadBGM(const std::wstring& filename)
{
	Sound& ins = Get();
	ins.m_seDict.emplace(filename, std::make_unique<SoundEffect>(ins.m_audEngine.get(), filename.c_str()));
	ins.m_bgmDict.emplace(filename, m_seDict[filename]->CreateInstance());
}

Sound& Sound::Get()
{
	static Sound ins;
	return ins;
}

void Sound::Update()
{
	Sound& ins = Get();

	if (!ins.m_audEngine->Update()) {
		// No audio device is active
		if (ins.m_audEngine->IsCriticalError()) {
		}
	}
}

void Sound::PlayOneShot(const std::wstring & soundName)
{
	Sound& ins = Get();
	ins.m_seDict[soundName]->Play();
}

void Sound::PlayBGM(const std::wstring & soundName)
{
	Sound& ins = Get();
	StopAllBGM();
	ins.m_bgmDict[soundName]->Play(true);
	ins.m_nowPlayingKey = soundName;
}

void Sound::StopAllBGM()
{
	Sound& ins = Get();
	for (auto& i : ins.m_bgmDict) {
		i.second->Stop();
	}
}

void Sound::PauseBGM()
{
	Sound& ins = Get();
	ins.m_bgmDict[ins.m_nowPlayingKey]->Pause();
}

void Sound::ResumeBGM()
{
	Sound& ins = Get();
	ins.m_bgmDict[ins.m_nowPlayingKey]->Resume();
}

