//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <d3dcompiler.h>

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
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;
}

// Draws the scene.
void Game::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0) {
		return;
	}

	Clear();

	static Vector3 pos(Vector3::Zero);
	const float Speed = 0.02f;
	
	float time = m_timer.GetTotalSeconds();

	if (GetKeyState('W') & 0x80) {
		pos += Speed * Vector3::Forward;
	}
	if (GetKeyState('A') & 0x80) {
		pos += Speed * Vector3::Left;
	}
	if (GetKeyState('S') & 0x80) {
		pos += Speed * Vector3::Backward;
	}
	if (GetKeyState('D') & 0x80) {
		pos += Speed * Vector3::Right;
	}
	if (GetKeyState('E') & 0x80) {
		pos += Speed * Vector3::Up;
	}
	if (GetKeyState('Q') & 0x80) {
		pos += Speed * Vector3::Down;
	}


	// 行列計算
	Matrix mWorld;
	Matrix mView;
	Matrix mProj;

	//ワールドトランスフォーム（絶対座標変換）
	mWorld = Matrix::CreateScale(1.0f, 1.0f, 1.0f)
		* Matrix()
		* Matrix::CreateTranslation(pos);

	// ビュートランスフォーム（視点座標変換）
	Vector3 eye(0.0f, 1.0f, 4.0f); //カメラ（視点）位置
	Vector3 lookat(0.0f, 0.0f, 0.0f);//注視位置
	Vector3 up(0.0f, 1.0f, 0.0f);//上方位置
	mView = Matrix::CreateLookAt(eye, lookat, up);


	// プロジェクショントランスフォーム（射影変換）
	int width, height;
	Game::GetDefaultSize(width, height);
	mProj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.0f, (float)width / (float)height, 0.1f, 1000.0f);

	// 使用シェーダー登録
	m_context.Get()->VSSetShader(m_vertexShader.Get(), NULL, 0);
	m_context.Get()->PSSetShader(m_pixelShader.Get(), NULL, 0);

	// コンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(m_context.Get()->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
		//ワールド、カメラ、射影行列を渡す
		cb.mW = XMMatrixTranspose(mWorld);
		cb.mWVP = XMMatrixTranspose(mWorld*mView*mProj);
		cb.mDiffuse = XMVectorSet(1.0f, 0.5f, 0.0f, 1.0f);
		Vector3 lightDir(1.0f, 1.0f, 0.0f);
		lightDir = Vector3::Transform(lightDir, Matrix::CreateRotationY(2.0f * time));
		cb.mLightDir = lightDir;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_context.Get()->Unmap(m_constantBuffer.Get(), 0);
	}

	//コンスタントバッファーをセット
	m_context.Get()->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
	m_context.Get()->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

	m_context.Get()->DrawIndexed(m_indexCount, 0, 0);

	Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
	// Clear the views.
	m_context->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
	m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

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

HRESULT Game::CompileShaderFromFile(WCHAR * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr)) {
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
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
	// 以下，追加した関数群


	// TODO : 例外処理が妥当か確認する．適当に書き直したので合っているかわからない
	HRESULT hr;


	// Compile the vertex shader
	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile(L"Tutorial02.fx", "VS", "vs_5_0", &pVSBlob);
	if (FAILED(hr)) {
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return DX::ThrowIfFailed(hr);
	}

	// Create the vertex shader

	hr = m_device.Get()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, m_vertexShader.GetAddressOf());
	if (FAILED(hr)) {
		pVSBlob->Release();
		return DX::ThrowIfFailed(hr);
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = m_device.Get()->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), m_vertexLayout.GetAddressOf());
	pVSBlob->Release();
	if (FAILED(hr))
		return DX::ThrowIfFailed(hr);

	// Set the input layout
	m_context.Get()->IASetInputLayout(m_vertexLayout.Get());

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile(L"Tutorial02.fx", "PS", "ps_5_0", &pPSBlob);
	if (FAILED(hr)) {
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return DX::ThrowIfFailed(hr);
	}

	// Create the pixel shader
	hr = m_device.Get()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, m_pixelShader.GetAddressOf());
	pPSBlob->Release();
	if (FAILED(hr))
		return DX::ThrowIfFailed(hr);


	// Create vertex buffer
	SimpleVertex vertices[] =
	{
		{  XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(-0.5f, -0.5f, -0.5f) },
		{  XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(-0.5f, -0.5f,  0.5f) },
		{  XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(-0.5f,  0.5f, -0.5f) },
		{  XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(-0.5f,  0.5f,  0.5f) },
		{  XMFLOAT3(0.5f, -0.5f, -0.5f) , XMFLOAT3(0.5f, -0.5f, -0.5f) },
		{  XMFLOAT3(0.5f, -0.5f,  0.5f) , XMFLOAT3(0.5f, -0.5f,  0.5f) },
		{  XMFLOAT3(0.5f,  0.5f, -0.5f) , XMFLOAT3(0.5f,  0.5f, -0.5f) },
		{  XMFLOAT3(0.5f,  0.5f,  0.5f) , XMFLOAT3(0.5f,  0.5f,  0.5f) },
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = m_device.Get()->CreateBuffer(&bd, &InitData, m_vertexBuffer.GetAddressOf());
	if (FAILED(hr))
		return DX::ThrowIfFailed(hr);


	// 頂点インデックスを定義
	static const unsigned short cubeIndices[] =
	{
		0,2,1, // -x
		1,2,3,

		4,5,6, // +x
		5,7,6,

		0,1,5, // -y
		0,5,4,

		2,6,7, // +y
		2,7,3,

		0,4,6, // -z
		0,6,2,

		1,3,7, // +z
		1,7,5,
	};


	// 頂点インデックスバッファ作成
	m_indexCount = ARRAYSIZE(cubeIndices);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);

	InitData.pSysMem = cubeIndices;
	hr = m_device.Get()->CreateBuffer(&indexBufferDesc, &indexBufferData, m_indexBuffer.GetAddressOf());
	if (FAILED(hr))
		return DX::ThrowIfFailed(hr);

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_context.Get()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

	// Set index buffer
	m_context.Get()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// Set primitive topology
	m_context.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//コンスタントバッファー作成　ここでは変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	{
		hr = m_device.Get()->CreateBuffer(&cb, NULL, m_constantBuffer.GetAddressOf());
		if (FAILED(hr)) {
			return DX::ThrowIfFailed(hr);
		}
	}

}

void Game::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.

	m_indexBuffer.Reset();
	m_vertexBuffer.Reset();
	m_vertexLayout.Reset();
	m_vertexShader.Reset();
	m_constantBuffer.Reset();
	m_pixelShader.Reset();

	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain.Reset();
	m_context.Reset();
	m_device.Reset();

	CreateDevice();

	CreateResources();
}