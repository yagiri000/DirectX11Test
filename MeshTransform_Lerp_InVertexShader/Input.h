#pragma once

#include <SimpleMath.h>
#include <Windows.h>
#include <memory>

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �L�[�{�[�h�A�}�E�X�̓��͂������N���X
// Update���ĂԑO��Initialize���Ă΂Ȃ��Ƃ����Ȃ�
// �܂��A���t���[��Update���Ă΂Ȃ��Ƃ����Ȃ�
class Input
{
public:
	enum class MouseButton
	{
		Left,
		Middle,
		Right
	};

	Input();
	Input& operator=(const Input& a) = delete;
	Input(const Input& a) = delete;

	static void Initialize(HWND window);

	static void Update();

	//�L�[��������Ă��邩��bool�ŕԂ�
	static bool GetKey(DirectX::Keyboard::Keys key);

	//�L�[�������ꂽ�t���[������true��Ԃ�
	static bool GetKeyDown(DirectX::Keyboard::Keys key);

	//�L�[�������ꂽ�t���[������true��Ԃ�
	static bool GetKeyUp(DirectX::Keyboard::Keys key);

	static bool GetMouseButton(MouseButton button);

	static bool GetMouseButtonDown(MouseButton button);

	static bool GetMouseButtonUp(MouseButton button);

	static Vector2 GetMouseDelta();

	static int GetMouseWheel();

private:
	static Input& Get();

	class Impl;
	std::unique_ptr<Impl> pImpl;
};