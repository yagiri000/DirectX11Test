#pragma once
#include "pch.h"
#include "StepTimer.h"
#include "MyVertexTypes.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Resource
{
public:
	Resource();
	Resource& operator=(const Resource& a) = delete;
	Resource(const Resource& a) = delete;
	static void OnInitialize(HWND window, int width, int height);
	static void OnWindowSizeChanged(int width, int height);
	static void OnCreateDevice();
	static void OnCreateResources();
	static void OnDeviceLost();

	static constexpr int DefaultWindowWidth = 1280;
	static constexpr int DefaultWindowHeight = 720;

	// Device resources.
	HWND                                            m_window;
	int                                             m_outputWidth;
	int                                             m_outputHeight;

	D3D_FEATURE_LEVEL                               m_featureLevel;
	ComPtr<ID3D11Device1>           m_device;
	ComPtr<ID3D11DeviceContext1>    m_context;

	ComPtr<IDXGISwapChain1>         m_swapChain;
	ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
	ComPtr<ID3D11DepthStencilView>  m_depthStencilView;
	ComPtr<ID3D11RasterizerState>	m_rasterizerState;
	ComPtr<ID3D11RasterizerState>	m_rasterizerStateWireFrame;
	ComPtr<ID3D11BlendState>		m_blendState;

	ComPtr<ID3D11VertexShader>		m_vertexShader;
	ComPtr<ID3D11GeometryShader>		m_geometoryShader;
	ComPtr<ID3D11PixelShader>	m_pixelShader;

	ComPtr<ID3D11ShaderResourceView> pShaderResView;
	ComPtr<ID3D11SamplerState> pSampler;
	ComPtr<ID3D11Resource> pTexture;

	static Resource& Get();
private:
};