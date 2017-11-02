#pragma once

#include <string>

class Sound
{
public:
	Sound();
	Sound& operator=(const Sound& a) = delete;
	Sound(const Sound& a) = delete;

	// 各種初期化を行う
	static void Initialize();

	// リソース(wavファイル)を読み込む
	static void Load();

	static void Update();

	static void PlayOneShot(const std::wstring& soundName);
	
	static void PlayBGM(const std::wstring& soundName);

	static void StopBGM();

	// TODO : 実装
	//static void PauseBGM();

	//static void UnPauseBGM();

private:
	static Sound& Get();
	std::unique_ptr<DirectX::AudioEngine> audEngine;
	std::unique_ptr<DirectX::SoundEffect> soundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> effect;
};