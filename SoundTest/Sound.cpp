#include "pch.h"
#include "Sound.h"

using namespace DirectX;

Sound::Sound()
{
}

void Sound::Initialize()
{
	Sound& ins = Get();

	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	ins.audEngine = std::make_unique<AudioEngine>(eflags);

	Load();
}

void Sound::Load()
{
	Sound& ins = Get();

	ins.soundEffect = std::make_unique<SoundEffect>(ins.audEngine.get(), L"Sound.wav");
	ins.effect = ins.soundEffect->CreateInstance();
}

Sound& Sound::Get()
{
	static Sound ins;
	return ins;
}

void Sound::Update()
{
	Sound& ins = Get();

	if (!ins.audEngine->Update()) {
		// No audio device is active
		if (ins.audEngine->IsCriticalError()) {
		}
	}
}

void Sound::PlayOneShot(const std::wstring & soundName)
{
	Sound& ins = Get();

	ins.soundEffect->Play();
}

void Sound::PlayBGM(const std::wstring & soundName)
{
}

void Sound::StopBGM()
{
}

