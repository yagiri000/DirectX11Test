#pragma once

#include <string>

class Sound
{
public:
	Sound();
	Sound& operator=(const Sound& a) = delete;
	Sound(const Sound& a) = delete;

	// �e�평�������s��
	static void Initialize();

	// ���\�[�X(wav�t�@�C��)��ǂݍ���
	static void Load();

	static void Update();

	static void PlayOneShot(const std::wstring& soundName);
	
	static void PlayBGM(const std::wstring& soundName);

	static void StopBGM();

	// TODO : ����
	//static void PauseBGM();

	//static void UnPauseBGM();

private:
	static Sound& Get();
	std::unique_ptr<DirectX::AudioEngine> audEngine;
	std::unique_ptr<DirectX::SoundEffect> soundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> effect;
};