#pragma once

#include <string>
#include <unordered_map>

class Sound
{
public:
	Sound();
	Sound& operator=(const Sound& a) = delete;
	Sound(const Sound& a) = delete;

	// �e�평�������s��
	static void Initialize();

	static void Load();

	// ���\�[�X(wav�t�@�C��)��SE�Ƃ��ēǂݍ���
	void LoadSE(const std::wstring & filename);

	// ���\�[�X(wav�t�@�C��)��BGM�Ƃ��ēǂݍ���
	void LoadBGM(const std::wstring & filename);

	static void Update();

	static void PlayOneShot(const std::wstring& soundName);

	// ���݂�BGM���~���A����BGM���J�n����
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