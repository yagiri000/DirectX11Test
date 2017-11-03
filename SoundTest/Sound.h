#pragma once

#include <string>
#include <unordered_map>

class Sound
{
public:
	Sound();
	Sound& operator=(const Sound& a) = delete;
	Sound(const Sound& a) = delete;

	// 各種初期化を行う
	static void Initialize();

	static void Load();

	// リソース(wavファイル)をSEとして読み込む
	void LoadSE(const std::wstring & filename);

	// リソース(wavファイル)をBGMとして読み込む
	void LoadBGM(const std::wstring & filename);

	static void Update();

	static void PlayOneShot(const std::wstring& soundName);

	// 現在のBGMを停止し、次のBGMを開始する
	static void PlayBGM(const std::wstring& soundName);

	static void StopAllBGM();

	static void PauseBGM();

	static void ResumeBGM();

private:
	static Sound& Get();
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;
	std::unordered_map<std::wstring, std::unique_ptr<DirectX::SoundEffect>> m_seDict;
	std::unordered_map<std::wstring, std::unique_ptr<DirectX::SoundEffectInstance>> m_bgmDict;
	std::wstring m_nowPlayingKey;
};