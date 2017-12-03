//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game() :
	m_window(nullptr),
	m_outputWidth(800),
	m_outputHeight(600),
	m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	m_window = window;
	m_outputWidth = std::max(width, 1);
	m_outputHeight = std::max(height, 1);

	CreateDevice();

	CreateResources();

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	*/
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
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
	float elapsedTime = float(timer.GetElapsedSeconds());


	// TODO: Add your game logic here.

	static const float Speed = 4.0f;

	if (GetKeyState('W') & 0x80) {
		m_pos += -Speed * Vector2::UnitY;
	}
	if (GetKeyState('A') & 0x80) {
		m_pos += -Speed * Vector2::UnitX;
	}
	if (GetKeyState('S') & 0x80) {
		m_pos += Speed * Vector2::UnitY;
	}
	if (GetKeyState('D') & 0x80) {
		m_pos += Speed * Vector2::UnitX;
	}
}

// Draws the scene.
void Game::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0) {
		return;
	}

	Clear();

	// TODO: Add your rendering code here.+
	m_spriteBatch->Begin();
	m_spriteBatch->Draw(m_texture.Get(), m_pos);
	m_spriteBatch->End();

	Present();

	// BloomTexture�쐬
	m_context->OMSetRenderTargets(1, m_bloomedRT.GetAddressOf(), nullptr);
	m_postProcess->SetBloomBlurParameters(true, 16.0f, 0.5f);
	m_postProcess->SetEffect(BasicPostProcess::BloomBlur);
	m_postProcess->SetSourceTexture(m_sceneSRV.Get());
	m_postProcess->Process(m_context.Get());



	// SceneTexture��BloomTexture��Combine
	// �L�[��������[Z] = SceneTexture, [X] = BloomTexture�̂ݕ\��
	int flag = 0;

	if (GetKeyState('Z') & 0x80) {
		flag = 1;
	}
	if (GetKeyState('X') & 0x80) {
		flag = 2;
	}
	if (flag == 0) {
		m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
		m_dualPostProcess->SetSourceTexture(m_sceneSRV.Get());
		m_dualPostProcess->SetSourceTexture2(m_bloomedSRV.Get());
		//m_dualPostProcess->SetBloomCombineParameters(0.2f, 1.0f, 0.3f, 1.0f);
		m_dualPostProcess->SetEffect(DualPostProcess::BloomCombine);
		m_dualPostProcess->Process(m_context.Get());
	}
	else if (flag == 1) {
		// SceneTexture�̂ݕ\��
		m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
		m_dualPostProcess->SetSourceTexture(m_sceneSRV.Get());
		m_dualPostProcess->SetSourceTexture2(m_bloomedSRV.Get());
		m_dualPostProcess->SetMergeParameters(1.0f, 0.0f);
		m_dualPostProcess->SetEffect(DualPostProcess::Merge);
		m_dualPostProcess->Process(m_context.Get());
	}
	else if (flag == 2) {
		// BloomTexture�̂ݕ\��
		m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
		m_dualPostProcess->SetSourceTexture(m_sceneSRV.Get());
		m_dualPostProcess->SetSourceTexture2(m_bloomedSRV.Get());
		m_dualPostProcess->SetMergeParameters(0.0f, 1.0f);
		m_dualPostProcess->SetEffect(DualPostProcess::Merge);
		m_dualPostProcess->Process(m_context.Get());
	}
}

// Helper method to clear the back buffers.
void Game::Clear()
{
	// Clear the views.
	//m_context->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
	//m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	m_context->ClearRenderTargetView(m_sceneRT.Get(), Colors::Black);
	m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_context->OMSetRenderTargets(1, m_sceneRT.GetAddressOf(), m_depthStencilView.Get());

	// Set the viewport.
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
	m_context->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_swapChain->Present(1, 0);

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
	m_outputWidth = std::max(width, 1);
	m_outputHeight = std::max(height, 1);

	CreateResources();

	// TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
	UINT creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static const D3D_FEATURE_LEVEL featureLevels[] =
	{
		// TODO: Modify for supported Direct3D feature levels
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// Create the DX11 API device object, and get a corresponding context.
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	DX::ThrowIfFailed(D3D11CreateDevice(
		nullptr,                            // specify nullptr to use the default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		device.ReleaseAndGetAddressOf(),    // returns the Direct3D device created
		&m_featureLevel,                    // returns feature level of device created
		context.ReleaseAndGetAddressOf()    // returns the device immediate context
	));

#ifndef NDEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(device.As(&d3dDebug))) {
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue))) {
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// TODO: Add more message IDs here as needed.
			};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	DX::ThrowIfFailed(device.As(&m_device));
	DX::ThrowIfFailed(context.As(&m_context));

	// TODO: Initialize device dependent objects here (independent of window size).
	CD3D11_TEXTURE2D_DESC sceneDesc(
		DXGI_FORMAT_R16G16B16A16_FLOAT, 800, 600,
		1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

	DX::ThrowIfFailed(
		m_device->CreateTexture2D(&sceneDesc, nullptr, m_sceneTex.GetAddressOf())
	);

	DX::ThrowIfFailed(
		m_device->CreateShaderResourceView(m_sceneTex.Get(), nullptr,
			m_sceneSRV.ReleaseAndGetAddressOf())
	);

	DX::ThrowIfFailed(
		m_device->CreateRenderTargetView(m_sceneTex.Get(), nullptr,
			m_sceneRT.ReleaseAndGetAddressOf()
		));


	DX::ThrowIfFailed(
		m_device->CreateTexture2D(&sceneDesc, nullptr, m_bloomedTex.GetAddressOf())
	);

	DX::ThrowIfFailed(
		m_device->CreateShaderResourceView(m_bloomedTex.Get(), nullptr,
			m_bloomedSRV.ReleaseAndGetAddressOf())
	);

	DX::ThrowIfFailed(
		m_device->CreateRenderTargetView(m_bloomedTex.Get(), nullptr,
			m_bloomedRT.ReleaseAndGetAddressOf()
		));

	m_spriteBatch = std::make_unique<SpriteBatch>(m_context.Get());
	ComPtr<ID3D11Resource> resource;
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_device.Get(), L"Brust01.png", resource.GetAddressOf(), m_texture.ReleaseAndGetAddressOf())
	);

	ComPtr<ID3D11Texture2D> texture;
	DX::ThrowIfFailed(resource.As(&texture));

	CD3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);

	m_postProcess = std::make_unique<BasicPostProcess>(m_device.Get());
	m_dualPostProcess = std::make_unique<DualPostProcess>(m_device.Get());
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
	// Clear the previous window size specific context.
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_context->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	m_renderTargetView.Reset();
	m_depthStencilView.Reset();
	m_context->Flush();

	UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
	UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	UINT backBufferCount = 2;

	// If the swap chain already exists, resize it, otherwise create one.
	if (m_swapChain) {
		HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			OnDeviceLost();

			// Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
			// and correctly set up the new device.
			return;
		}
		else {
			DX::ThrowIfFailed(hr);
		}
	}
	else {
		// First, retrieve the underlying DXGI Device from the D3D Device.
		ComPtr<IDXGIDevice1> dxgiDevice;
		DX::ThrowIfFailed(m_device.As(&dxgiDevice));

		// Identify the physical adapter (GPU or card) this device is running on.
		ComPtr<IDXGIAdapter> dxgiAdapter;
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

		// And obtain the factory object that created it.
		ComPtr<IDXGIFactory2> dxgiFactory;
		DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		// Create a descriptor for the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = backBufferWidth;
		swapChainDesc.Height = backBufferHeight;
		swapChainDesc.Format = backBufferFormat;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = backBufferCount;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
		fsSwapChainDesc.Windowed = TRUE;

		// Create a SwapChain from a Win32 window.
		DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
			m_device.Get(),
			m_window,
			&swapChainDesc,
			&fsSwapChainDesc,
			nullptr,
			m_swapChain.ReleaseAndGetAddressOf()
		));

		// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
		DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
	}

	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	// Create a view interface on the rendertarget to use on bind.
	DX::ThrowIfFailed(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(m_device->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(m_device->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

	// TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.

	m_postProcess.reset();
	m_dualPostProcess.reset();
	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain.Reset();
	m_context.Reset();
	m_device.Reset();
	m_texture.Reset();
	m_spriteBatch.reset();

	CreateDevice();

	CreateResources();
}