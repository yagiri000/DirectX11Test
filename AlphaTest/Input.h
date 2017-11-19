#pragma once

#include <SimpleMath.h>
#include <Windows.h>
#include <memory>

using namespace DirectX;
using namespace DirectX::SimpleMath;

// キーボード、マウスの入力を扱うクラス
// Updateを呼ぶ前にInitializeを呼ばないといけない
// また、毎フレームUpdateを呼ばないといけない
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

	//キーが押されているかをboolで返す
	static bool GetKey(DirectX::Keyboard::Keys key);

	//キーが押されたフレームだけtrueを返す
	static bool GetKeyDown(DirectX::Keyboard::Keys key);

	//キーが離されたフレームだけtrueを返す
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