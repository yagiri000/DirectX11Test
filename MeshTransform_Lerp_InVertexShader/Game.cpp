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
#include "Input.h"

extern void ExitGame();

using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

constexpr UINT Game::Columns;
constexpr UINT Game::Rows;


struct BinFile
{
	BinFile(const wchar_t* fpath)
	{
		std::ifstream binfile(fpath, std::ios::in | std::ios::binary);

		if (binfile.is_open()) {
			int fsize = static_cast<int>(binfile.seekg(0, std::ios::end).tellg());
			binfile.seekg(0, std::ios::beg);
			std::unique_ptr<char> code(new char[fsize]);
			binfile.read(code.get(), fsize);
			nSize = fsize;
			Bin = std::move(code);
		}

	}

	const void* get() const { return Bin.get(); }
	int size() const { return nSize; }
private:
	int nSize = 0;
	std::unique_ptr<char> Bin;
};

Game::Game() :
	m_window(nullptr),
	m_outputWidth(1280),
	m_outputHeight(720),
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
	Input::Update();
	float deltaTime = float(timer.GetElapsedSeconds());

	
	static const auto Lerp = [](float a, float b, float rate) {
		rate = pow(rate, 0.75f);
		return a * (1.0f - rate) + b * rate;
	};

	if (Input::GetKeyDown(Keyboard::X)) {
		m_context->RSSetState(m_rasterizerState.Get());
	}
	if (Input::GetKeyDown(Keyboard::C)) {
		m_context->RSSetState(m_rasterizerStateWireFrame.Get());
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

	// TODO: Add your rendering code here.
	// Render a triangle

	static float elapsed = 0.0f;
	elapsed += m_timer.GetElapsedSeconds();
	if (Input::GetKeyDown(Keyboard::Z)) {
		elapsed = 0.0f;
	}

	static Vector3 pos = Vector3::Zero;
	static constexpr float Speed = 0.03f;

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

	// ビュートランスフォーム（視点座標変換）

	Vector3 eye(0.0f, 1.0f, 3.0f); //カメラ（視点）位置
	Vector3 lookat(0.0f, 0.0f, 0.0f);//注視位置
	Vector3 up(0.0f, 1.0f, 0.0f);//上方位置
	mView = Matrix::CreateLookAt(eye, lookat, up);
	Matrix dirMat = Matrix::CreateWorld(Vector3::Zero, Vector3(0.0f, 0.0f, -0.01f) - pos, Vector3::Up);
	//Quaternion q = Quaternion::CreateFromRotationMatrix(dirMat);
	//ワールドトランスフォーム（絶対座標変換）
	mWorld = Matrix::CreateScale(1.0f, 1.0f, 1.0f) * Matrix()
		* Matrix::CreateTranslation(pos);

	// プロジェクショントランスフォーム（射影変換）
	int width, height;
	Game::GetDefaultSize(width, height);
	mProj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.0f, (float)width / (float)height, 0.1f, 1000.0f);

	// 使用シェーダー登録
	m_context.Get()->VSSetShader(m_vertexShader.Get(), NULL, 0);
	m_context.Get()->PSSetShader(m_pixelShader.Get(), NULL, 0);

	// サンプラー
	UINT smp_slot = 0;
	ID3D11SamplerState* smp[1] = { pSampler.Get() };
	m_context->VSSetSamplers(smp_slot, 1, smp);
	m_context->PSSetSamplers(smp_slot, 1, smp);

	// シェーダーリソースビュー（テクスチャ）
	UINT srv_slot = 0;
	ID3D11ShaderResourceView* srv[1] = { pShaderResView.Get() };
	m_context->VSSetShaderResources(srv_slot, 1, srv);
	m_context->PSSetShaderResources(srv_slot, 1, srv);

	// コンスタントバッファーに各種データを渡す
	{
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_VERTEX_CONSTANT_BUFFER cb;
		if (SUCCEEDED(m_context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
			//ワールド、カメラ、射影行列を渡す
			XMMATRIX m = mWorld*mView*mProj;
			m = XMMatrixTranspose(m);

			cb.mW = mWorld.Transpose();
			cb.mWVP = m;
			cb.mUV = XMVectorSet(elapsed, 0.0f, 0.0f, 0.0f);
			static constexpr float lifeTime = 1.0f;
			float life = Utility::Clamp(elapsed / lifeTime, 0.0f, 1.0f);
			cb.mLife = XMVectorSet(life, 0.0f, 0.0f, 0.0f);

			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			m_context->Unmap(m_constantBuffer.Get(), 0);
		}
	}
	m_context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());//バーテックスバッファーで使う
	m_context->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());//ピクセルシェーダーでの使う

	m_context.Get()->DrawIndexed(m_indexCount, 0, 0);

	Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
	// Clear the views.
	// m_context->ClearRenderTargetView(m_renderTargetView.Get(), Color(0.1f, 0.1f, 0.1f));
	float a[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_context->ClearRenderTargetView(m_renderTargetView.Get(), a);
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
	width = 1280;
	height = 720;
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

	Input::Initialize(m_window);


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

	// ブレンドステート作成
	// TODO : 加算にしたつもりだが正しく動作するか検証する
	{
		D3D11_BLEND_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
		bd.IndependentBlendEnable = false;
		bd.AlphaToCoverageEnable = false;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		if (FAILED(m_device->CreateBlendState(&bd, m_blendState.GetAddressOf()))) {
			return DX::ThrowIfFailed(E_FAIL);
		}

		UINT mask = 0xffffffff;
		m_context->OMSetBlendState(m_blendState.Get(), NULL, mask);
	}

	// TODO: Initialize windows-size dependent objects here.
	// 以下，追加した関数群


	// TODO : 例外処理が妥当か確認する．適当に書き直したので合っているかわからない
	HRESULT hr;


	//シェーダー読み込み
	BinFile vscode(L"..\\Debug\\VertexShader.cso");
	BinFile pscode(L"..\\Debug\\PixelShader.cso");

	// 頂点シェーダ作成
	//  メモ：シェーダーをデバッグ情報ありでコンパイルすると
	//　　　　ここでエラー発生　CREATEVERTEXSHADER_INVALIDSHADERBYTECODE
	hr = m_device.Get()->CreateVertexShader(vscode.get(), vscode.size(), NULL, m_vertexShader.GetAddressOf());
	if (FAILED(hr)) {
		return DX::ThrowIfFailed(hr);
	}

	// ピクセルシェーダ作成
	hr = m_device.Get()->CreatePixelShader(pscode.get(), pscode.size(), NULL, m_pixelShader.GetAddressOf());
	if (FAILED(hr)) {
		return DX::ThrowIfFailed(hr);
	}

	// 入力レイアウト定義
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // NORMALとして使用せず，Lerp先を設定する
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT elem_num = ARRAYSIZE(layout);

	// 入力レイアウト作成
	hr = m_device.Get()->CreateInputLayout(layout, elem_num, vscode.get(),
		vscode.size(), m_vertexLayout.GetAddressOf());
	if (FAILED(hr)) {
		return DX::ThrowIfFailed(hr);
	}

	// Set the input layout
	m_context.Get()->IASetInputLayout(m_vertexLayout.Get());


	// テクスチャ作成
	hr = DirectX::CreateWICTextureFromFile(m_device.Get(), L"image.png", &pTexture, &pShaderResView);
	if (FAILED(hr)) {
		return DX::ThrowIfFailed(hr);
	}

	// サンプラー作成
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = m_device->CreateSamplerState(&sampDesc, &pSampler);
	if (FAILED(hr)) {
		return DX::ThrowIfFailed(hr);
	}


	static const auto Lerp = [](float a, float b, float rate) {
		return a * (1.0f - rate) + b * rate;
	};

	static constexpr float R0 = 0.5f * 0.8f;
	static constexpr float R1 = 1.0f * 0.8f;
	static constexpr UINT ColumnsPlus = Columns + 1;
	static constexpr UINT num = (ColumnsPlus * Rows);

	SimpleVertex vertices[num];

	for (UINT i = 0; i < ColumnsPlus; i++) {
		for (UINT j = 0; j < Rows; j++) {
			UINT num = i * Rows + j;
			float x = (float)i / Columns;
			float y = (float)j / (Rows - 1);
			float angle = XM_2PI * x;
			vertices[num].Pos = XMFLOAT3(cos(angle) * Lerp(R0, R1, y), sin(angle) * Lerp(R0, R1, y), 0.0f);
			vertices[num].Normal = XMFLOAT3(cos(angle) * 1.0f, sin(angle) * 1.0f, 0.0f);
			vertices[num].UV = XMFLOAT2(x * 4.0f, y);
			float alpha;
			if (y < 0.5f) {
				alpha = 2.0f * y;
			}
			else {
				alpha = 1.0f - 2.0 * (y - 0.5f);
			}
			vertices[num].Color = XMFLOAT4(1.0f, 0.1f, 0.0f, alpha*alpha);
		}
	}


	// メッシュのインデックスを読み込みます。インデックスの 3 つ 1 組の値のそれぞれは、次のものを表します
	// 画面上に描画される三角形を表します。
	// たとえば、0,2,1 とは、頂点バッファーからのインデックスを意味します:
	// 0、2、1 を持つ頂点が、このメッシュの
	// 最初の三角形を構成することを意味します。
	static unsigned short cubeIndices[Columns*(Rows - 1) * 6];
	int count = 0;
	for (UINT i = 0; i < Columns; i++) {
		for (UINT j = 0; j < Rows - 1; j++) {
			int n = j + i * Rows;
			int nPlusOne = j + 1 + i * Rows;
			cubeIndices[count++] = n % num;
			cubeIndices[count++] = (n + Rows) % num;
			cubeIndices[count++] = (nPlusOne) % num;
			cubeIndices[count++] = (nPlusOne) % num;
			cubeIndices[count++] = (n + Rows) % num;
			cubeIndices[count++] = (nPlusOne + Rows) % num;
		}
	}


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(SimpleVertex) * num;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = m_device.Get()->CreateBuffer(&bd, &InitData, m_vertexBuffer.GetAddressOf());
	if (FAILED(hr))
		return DX::ThrowIfFailed(hr);

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_context.Get()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);


	m_indexCount = ARRAYSIZE(cubeIndices);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		m_device->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&m_indexBuffer
		)
	);

	m_context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // 各インデックスは、1 つの 16 ビット符号なし整数 (short) です。
		0
	);

	// Set primitive topology
	m_context.Get()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//コンスタントバッファー作成　バーテックスシェーダー用
	{
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(SIMPLESHADER_VERTEX_CONSTANT_BUFFER);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.StructureByteStride = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;

		hr = m_device.Get()->CreateBuffer(&cb, NULL, m_constantBuffer.GetAddressOf());
		if (FAILED(hr)) {
			return DX::ThrowIfFailed(hr);
		}
	}


	//ラスタライズ設定
	{
		D3D11_RASTERIZER_DESC rdc;
		ZeroMemory(&rdc, sizeof(rdc));
		rdc.CullMode = D3D11_CULL_NONE;
		rdc.FillMode = D3D11_FILL_SOLID;
		rdc.FrontCounterClockwise = TRUE;

		m_device->CreateRasterizerState(&rdc, m_rasterizerState.GetAddressOf());
		m_context->RSSetState(m_rasterizerState.Get());
	}
	{
		D3D11_RASTERIZER_DESC rdc;
		ZeroMemory(&rdc, sizeof(rdc));
		rdc.CullMode = D3D11_CULL_NONE;
		rdc.FillMode = D3D11_FILL_WIREFRAME;
		rdc.FrontCounterClockwise = TRUE;

		m_device->CreateRasterizerState(&rdc, m_rasterizerStateWireFrame.GetAddressOf());
	}
}

void Game::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.


	pTexture.Reset();
	pShaderResView.Reset();
	pSampler.Reset();

	m_constantBuffer.Reset();
	m_vertexShader.Reset();
	m_pixelShader.Reset();
	m_vertexLayout.Reset();
	m_vertexBuffer.Reset();

	m_blendState.Reset();
	m_rasterizerState.Reset();
	m_rasterizerStateWireFrame.Reset();
	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain.Reset();

	m_context.Reset();
	m_device.Reset();


	CreateDevice();

	CreateResources();
}