#include "pch.h"
#include "ParticleSystem.h"
#include "SimpleMath.h"
#include "Random.h"
#include <string>
#include "Utility.h"
#include "Resource.h"
#include "Binfile.h"

constexpr int Resource::DefaultWindowWidth;
constexpr int Resource::DefaultWindowHeight;
constexpr int Resource::ParticleMax;

// TODO : �O�����璼��m_xxxx�ɃA�N�Z�X���Ă��镔���͂�������

Resource::Resource() :
	m_window(nullptr),
	m_outputWidth(DefaultWindowWidth),
	m_outputHeight(DefaultWindowHeight),
	m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

void Resource::OnInitialize(HWND window, int width, int height)
{
	auto& ins = Get();
	ins.m_window = window;
	ins.m_outputWidth = std::max(width, 1);
	ins.m_outputHeight = std::max(height, 1);
}

void Resource::OnWindowSizeChanged(int width, int height)
{
	auto& ins = Get();
	ins.m_outputWidth = std::max(width, 1);
	ins.m_outputHeight = std::max(height, 1);
}

void Resource::OnCreateDevice()
{
	auto& ins = Get();
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
		&ins.m_featureLevel,                    // returns feature level of device created
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

	DX::ThrowIfFailed(device.As(&ins.m_device));
	DX::ThrowIfFailed(context.As(&ins.m_context));
}

void Resource::OnCreateResources()
{
	auto& ins = Get();
	// Clear the previous window size specific context.
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	ins.m_context->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	ins.m_renderTargetView.Reset();
	ins.m_depthStencilView.Reset();
	ins.m_context->Flush();

	UINT backBufferWidth = static_cast<UINT>(ins.m_outputWidth);
	UINT backBufferHeight = static_cast<UINT>(ins.m_outputHeight);
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	UINT backBufferCount = 2;

	// If the swap chain already exists, resize it, otherwise create one.
	if (ins.m_swapChain) {
		HRESULT hr = ins.m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

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
		DX::ThrowIfFailed(ins.m_device.As(&dxgiDevice));

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
			ins.m_device.Get(),
			ins.m_window,
			&swapChainDesc,
			&fsSwapChainDesc,
			nullptr,
			ins.m_swapChain.ReleaseAndGetAddressOf()
		));

		// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
		DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(ins.m_window, DXGI_MWA_NO_ALT_ENTER));
	}

	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(ins.m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	// Create a view interface on the rendertarget to use on bind.
	DX::ThrowIfFailed(ins.m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, ins.m_renderTargetView.ReleaseAndGetAddressOf()));

	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(ins.m_device->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(ins.m_device->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, ins.m_depthStencilView.ReleaseAndGetAddressOf()));

	// �u�����h�X�e�[�g�쐬
	// TODO : ���Z�ɂ������肾�����������삷�邩���؂���
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

		if (FAILED(ins.m_device->CreateBlendState(&bd, ins.m_blendState.GetAddressOf()))) {
			return DX::ThrowIfFailed(E_FAIL);
		}

	}

	// TODO: Initialize windows-size dependent objects here.
	// �ȉ��C�ǉ������֐��Q

	//���X�^���C�Y�ݒ�
	{
		D3D11_RASTERIZER_DESC rdc;
		ZeroMemory(&rdc, sizeof(rdc));
		rdc.CullMode = D3D11_CULL_NONE;
		rdc.FillMode = D3D11_FILL_SOLID;
		rdc.FrontCounterClockwise = TRUE;

		ins.m_device->CreateRasterizerState(&rdc, ins.m_rasterizerState.GetAddressOf());
	}

	// TODO : ��O�������Ó����m�F����D�K���ɏ����������̂ō����Ă��邩�킩��Ȃ�
	HRESULT hr;


	//�V�F�[�_�[�ǂݍ���
	static const std::wstring CompiledFolder = L"..\\Debug\\";
	BinFile cscode((CompiledFolder + L"ComputeShader.cso").c_str());
	BinFile vscode((CompiledFolder + L"VertexShader.cso").c_str());
	BinFile gscode((CompiledFolder + L"GeometryShader.cso").c_str());
	BinFile pscode((CompiledFolder + L"PixelShader.cso").c_str());

	// �e�V�F�[�_�쐬
	//  �����F�V�F�[�_�[���f�o�b�O��񂠂�ŃR���p�C�������
	//�@�@�@�@�����ŃG���[�����@CREATEVERTEXSHADER_INVALIDSHADERBYTECODE
	hr = ins.m_device.Get()->CreateComputeShader(cscode.get(), cscode.size(), NULL, ins.m_computeShader.GetAddressOf());
	if (FAILED(hr)) {
		return DX::ThrowIfFailed(hr);
	}

	hr = ins.m_device.Get()->CreateVertexShader(vscode.get(), vscode.size(), NULL, ins.m_vertexShader.GetAddressOf());
	if (FAILED(hr)) {
		return DX::ThrowIfFailed(hr);
	}

	hr = ins.m_device.Get()->CreateGeometryShader(gscode.get(), gscode.size(), NULL, ins.m_geometoryShader.GetAddressOf());
	if (FAILED(hr)) {
		return DX::ThrowIfFailed(hr);
	}

	hr = ins.m_device.Get()->CreatePixelShader(pscode.get(), pscode.size(), NULL, ins.m_pixelShader.GetAddressOf());
	if (FAILED(hr)) {
		return DX::ThrowIfFailed(hr);
	}


	// �e�N�X�`���쐬
	hr = DirectX::CreateWICTextureFromFile(ins.m_device.Get(), L"image.png", &ins.pTexture, &ins.pShaderResView);
	if (FAILED(hr)) {
		return DX::ThrowIfFailed(hr);
	}

	// �T���v���[�쐬
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = ins.m_device->CreateSamplerState(&sampDesc, &ins.pSampler);
	if (FAILED(hr)) {
		return DX::ThrowIfFailed(hr);
	}


	// �R���X�^���g�o�b�t�@�[�쐬�@�V�F�[�_�[�ɕϊ��s���n���p

	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	{
		hr = ins.m_device.Get()->CreateBuffer(&cb, NULL, ins.m_constantBuffer.GetAddressOf());
		if (FAILED(hr)) {
			return DX::ThrowIfFailed(hr);
		}
	}

	// Structured Buffers�쐬�̂��߂̃_�~�[�̔z��
	static std::vector<ParticlePoint> particleArray;
	particleArray.resize(ParticleMax);
	
	// Create Structured Buffers
	CreateStructuredBuffer< ParticlePoint >(
		ins.m_device.Get(), 
		ParticleMax, 
		ins.m_particles.GetAddressOf(), 
		ins.m_particlesSRV.GetAddressOf(), 
		ins.m_particlesUAV.GetAddressOf(), 
		&particleArray[0]);
}

void Resource::OnDeviceLost()
{
	auto& ins = Get();
	ins.pTexture.Reset();
	ins.pShaderResView.Reset();
	ins.pSampler.Reset();
	ins.m_constantBuffer.Reset();

	ins.m_computeShader.Reset();
	ins.m_vertexShader.Reset();
	ins.m_geometoryShader.Reset();
	ins.m_pixelShader.Reset();

	ins.m_blendState.Reset();
	ins.m_rasterizerState.Reset();
	ins.m_rasterizerStateWireFrame.Reset();
	ins.m_depthStencilView.Reset();
	ins.m_renderTargetView.Reset();
	ins.m_swapChain.Reset();

	ins.m_context.Reset();
	ins.m_device.Reset();
}

Resource& Resource::Get()
{
	static Resource ins;
	return ins;
}