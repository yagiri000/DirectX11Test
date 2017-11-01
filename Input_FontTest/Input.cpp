#include "pch.h"
#include "Input.h"


Input::Input()
{
}

void Input::Initialize(HWND window)
{
	Input& input = Get();
	input.m_keyboard = std::make_unique<DirectX::Keyboard>();
	input.m_mouse = std::make_unique<DirectX::Mouse>();
	input.m_keyboardTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	input.m_mouseTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
	input.m_mouse->SetWindow(window);
}

Input& Input::Get()
{
	static Input ins;
	return ins;
}

void Input::Update()
{
	Input& input = Get();
	input.m_keyboardState = input.m_keyboard->GetState();
	input.m_mouseState = input.m_mouse->GetState();
	input.m_keyboardTracker->Update(input.m_keyboardState);
	input.m_mouseTracker->Update(input.m_mouseState);
}

//キーが押されているかをboolで返す
bool Input::GetKey(DirectX::Keyboard::Keys key)
{
	Input& input = Get();
	return input.m_keyboardState.IsKeyDown(key);
}

//キーが押されたフレームだけtrueを返す
bool Input::GetKeyDown(DirectX::Keyboard::Keys key)
{
	Input& input = Get();
	return input.m_keyboardTracker->IsKeyPressed(key);

}

//キーが離されたフレームだけtrueを返す
bool Input::GetKeyUp(DirectX::Keyboard::Keys key)
{
	Input& input = Get();
	return input.m_keyboardTracker->IsKeyReleased(key);
}


bool Input::GetMouseButton(MouseButton button)
{
	Input& input = Get();
	if (button == MouseButton::Left) {
		return input.m_mouseState.leftButton;
	}
	else if (button == MouseButton::Middle) {
		return input.m_mouseState.middleButton;
	}
	else if (button == MouseButton::Right) {
		return input.m_mouseState.rightButton;
	}
	return false;
}

bool Input::GetMouseButtonDown(MouseButton button)
{
	using ButtonState = Mouse::ButtonStateTracker::ButtonState;
	Input& input = Get();
	if (button == MouseButton::Left) {
		return input.m_mouseTracker->leftButton == ButtonState::PRESSED;
	}
	else if (button == MouseButton::Middle) {
		return input.m_mouseTracker->middleButton == ButtonState::PRESSED;
	}
	else if (button == MouseButton::Right) {
		return input.m_mouseTracker->rightButton == ButtonState::PRESSED;
	}
	return false;
}

bool Input::GetMouseButtonUp(MouseButton button)
{
	using ButtonState = Mouse::ButtonStateTracker::ButtonState;
	Input& input = Get();
	if (button == MouseButton::Left) {
		return input.m_mouseTracker->leftButton == ButtonState::UP;
	}
	else if (button == MouseButton::Middle) {
		return input.m_mouseTracker->middleButton == ButtonState::UP;
	}
	else if (button == MouseButton::Right) {
		return input.m_mouseTracker->rightButton == ButtonState::UP;
	}
	return false;
}


Vector2 Input::GetMousePos()
{
	Input& input = Get();
	return Vector2(
		static_cast<float>(input.m_mouseState.x),
		static_cast<float>(input.m_mouseState.y)
	);
}

int Input::GetMouseWheel()
{
	static int preWheelValue = 0;
	Input& input = Get();
	int scrollWheelValue = input.m_mouseState.scrollWheelValue;
	int delta = scrollWheelValue - preWheelValue;
	preWheelValue = scrollWheelValue;
	return delta;
}


