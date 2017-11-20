//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <d3dcompiler.h>
#include <algorithm>
#include <memory>
#include <fstream>
#include "Utility.h"
#include "Random.h"
#include "Font.h"
#include "Resource.h"

extern void ExitGame();

using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;





Game::Game()
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	Resource::OnInitialize(window, width, height);
	CreateDevice();
	CreateResources();


	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

// Executes the basic game loop.
void Game::Tick()
{
	m_timer.Tick([&]() {
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	float deltaTime = float(timer.GetElapsedSeconds());

	m_particleSystem->Update(deltaTime);
}

// Draws the scene.
void Game::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0) {
		return;
	}

	Clear();

	m_particleSystem->Render();

	Font::DrawQueue(L"FPS : " + std::to_wstring(m_timer.GetFramesPerSecond()), Vector2(20.0f, 20.0f));
	Font::DrawQueue(L"NUM : " + std::to_wstring(999), Vector2(20.0f, 50.0f));
	Font::Batch();

	Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
	auto& res = Resource::Get();

	// Clear the views.
	// m_context->ClearRenderTargetView(m_renderTargetView.Get(), Color(0.1f, 0.1f, 0.1f));
	res.m_context->ClearRenderTargetView(res.m_renderTargetView.Get(), Colors::Black);
	res.m_context->ClearDepthStencilView(res.m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	res.m_context->OMSetRenderTargets(1, res.m_renderTargetView.GetAddressOf(), res.m_depthStencilView.Get());

	// Set the viewport.
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(res.m_outputWidth), static_cast<float>(res.m_outputHeight));
	res.m_context->RSSetViewports(1, &viewport);


}

// Presents the back buffer contents to the screen.
void Game::Present()
{
	auto& res = Resource::Get();
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = res.m_swapChain->Present(1, 0);

	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
		OnDeviceLost();
	}
	else {
		DX::ThrowIfFailed(hr);
	}
}

// Message handlers
void Game::OnActivated()
{
	// TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
	Resource::OnWindowSizeChanged(width, height);

	CreateResources();

	// TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = Resource::DefaultWindowWidth;
	height = Resource::DefaultWindowHeight;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
	Resource::OnCreateDevice();

	// TODO: Initialize device dependent objects here (independent of window size).



}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
	auto& res = Resource::Get();
	Resource::OnCreateResources();

	Font::Initialize(res.m_device.Get(), res.m_context.Get(), L"myfile.spritefont");

	m_particleSystem = std::make_unique<ParticleSystem>();
	m_particleSystem->OnInitialize();
}

void Game::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.

	Font::OnDeviceLost();
	m_particleSystem->OnDeviceLost();

	Resource::OnDeviceLost();

	CreateDevice();

	CreateResources();
}