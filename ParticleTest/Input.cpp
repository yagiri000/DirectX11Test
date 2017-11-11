#include "pch.h"
#include "Input.h"


class Input::Impl
{
public:
	Impl(HWND window);

	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	std::unique_ptr<Keyboard::KeyboardStateTracker> m_keyboardTracker;
	std::unique_ptr<Mouse::ButtonStateTracker> m_mouseTracker;

	// TODO : unique_ptrにするべきか？
	Keyboard::State m_keyboardState;
	Mouse::State m_mouseState;
	bool isFirstFrame;
};


Input::Impl::Impl(HWND window) :
	m_keyboard(std::make_unique<DirectX::Keyboard>()),
	m_mouse(std::make_unique<DirectX::Mouse>()),
	m_keyboardTracker(std::make_unique<DirectX::Keyboard::KeyboardStateTracker>()),
	m_mouseTracker(std::make_unique<DirectX::Mouse::ButtonStateTracker>()),
	isFirstFrame(false)
{
	m_mouse->SetWindow(window);
}


Input::Input()
{
}

void Input::Initialize(HWND window)
{
	Input& ins = Get();
	ins.pImpl = std::make_unique<Impl>(window);
}

Input& Input::Get()
{
	static Input ins;
	return ins;
}

void Input::Update()
{
	auto& pImpl = Get().pImpl;
	pImpl->m_keyboardState = pImpl->m_keyboard->GetState();
	pImpl->m_mouseState = pImpl->m_mouse->GetState();
	pImpl->m_keyboardTracker->Update(pImpl->m_keyboardState);
	pImpl->m_mouseTracker->Update(pImpl->m_mouseState);
}

//キーが押されているかをboolで返す
bool Input::GetKey(DirectX::Keyboard::Keys key)
{
	auto& pImpl = Get().pImpl;
	return pImpl->m_keyboardState.IsKeyDown(key);
}

//キーが押されたフレームだけtrueを返す
bool Input::GetKeyDown(DirectX::Keyboard::Keys key)
{
	auto& pImpl = Get().pImpl;
	return pImpl->m_keyboardTracker->IsKeyPressed(key);

}

//キーが離されたフレームだけtrueを返す
bool Input::GetKeyUp(DirectX::Keyboard::Keys key)
{
	auto& pImpl = Get().pImpl;
	return pImpl->m_keyboardTracker->IsKeyReleased(key);
}


bool Input::GetMouseButton(MouseButton button)
{
	auto& pImpl = Get().pImpl;
	if (button == MouseButton::Left) {
		return pImpl->m_mouseState.leftButton;
	}
	else if (button == MouseButton::Middle) {
		return pImpl->m_mouseState.middleButton;
	}
	else if (button == MouseButton::Right) {
		return pImpl->m_mouseState.rightButton;
	}
	return false;
}

bool Input::GetMouseButtonDown(MouseButton button)
{
	using ButtonState = Mouse::ButtonStateTracker::ButtonState;

	auto& pImpl = Get().pImpl;
	if (button == MouseButton::Left) {
		return pImpl->m_mouseTracker->leftButton == ButtonState::PRESSED;
	}
	else if (button == MouseButton::Middle) {
		return pImpl->m_mouseTracker->middleButton == ButtonState::PRESSED;
	}
	else if (button == MouseButton::Right) {
		return pImpl->m_mouseTracker->rightButton == ButtonState::PRESSED;
	}
	return false;
}

bool Input::GetMouseButtonUp(MouseButton button)
{
	using ButtonState = Mouse::ButtonStateTracker::ButtonState;
	auto& pImpl = Get().pImpl;
	if (button == MouseButton::Left) {
		return pImpl->m_mouseTracker->leftButton == ButtonState::UP;
	}
	else if (button == MouseButton::Middle) {
		return pImpl->m_mouseTracker->middleButton == ButtonState::UP;
	}
	else if (button == MouseButton::Right) {
		return pImpl->m_mouseTracker->rightButton == ButtonState::UP;
	}
	return false;
}


Vector2 Input::GetMousePos()
{
	
	auto& pImpl = Get().pImpl;
	if (pImpl->isFirstFrame) {
		pImpl->isFirstFrame = false;
		return Vector2::Zero;
	}
	return Vector2(
		static_cast<float>(pImpl->m_mouseState.x),
		static_cast<float>(pImpl->m_mouseState.y)
	);
}

int Input::GetMouseWheel()
{
	static int preWheelValue = 0;
	auto& pImpl = Get().pImpl;
	int scrollWheelValue = pImpl->m_mouseState.scrollWheelValue;
	int delta = scrollWheelValue - preWheelValue;
	preWheelValue = scrollWheelValue;
	return delta;
}
