#pragma once
#include "pch.h"
#include "StepTimer.h"
#include "MyVertexTypes.h"
#include <vector>

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

	ComPtr<ID3D11ComputeShader>		m_computeShader;
	ComPtr<ID3D11VertexShader>		m_vertexShader;
	ComPtr<ID3D11GeometryShader>		m_geometoryShader;
	ComPtr<ID3D11PixelShader>	m_pixelShader;

	ComPtr<ID3D11ShaderResourceView> pShaderResView;
	ComPtr<ID3D11SamplerState> pSampler;
	ComPtr<ID3D11Resource> pTexture;

	// パーティクル用リソース
	static constexpr int ParticleMax = 10000;
	ComPtr<ID3D11Buffer>			m_constantBuffer;
	ComPtr<ID3D11Buffer>					m_particles;
	ComPtr<ID3D11ShaderResourceView>		m_particlesSRV;
	ComPtr<ID3D11UnorderedAccessView>		m_particlesUAV;

	static Resource& Get();
private:
};

//--------------------------------------------------------------------------------------
// Helper for creating structured buffers with an SRV and UAV
//--------------------------------------------------------------------------------------
template <class T>
HRESULT CreateStructuredBuffer(ID3D11Device* pd3dDevice, UINT iNumElements, ID3D11Buffer** ppBuffer, ID3D11ShaderResourceView** ppSRV, ID3D11UnorderedAccessView** ppUAV, const T* pInitialData = NULL)
{
	HRESULT hr = S_OK;

	// Create SB
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = iNumElements * sizeof(T);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.StructureByteStride = sizeof(T);

	D3D11_SUBRESOURCE_DATA bufferInitData;
	ZeroMemory(&bufferInitData, sizeof(bufferInitData));
	bufferInitData.pSysMem = pInitialData;
	pd3dDevice->CreateBuffer(&bufferDesc, (pInitialData) ? &bufferInitData : NULL, ppBuffer);

	// Create SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = iNumElements;
	pd3dDevice->CreateShaderResourceView(*ppBuffer, &srvDesc, ppSRV);

	// Create UAV
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = iNumElements;
	pd3dDevice->CreateUnorderedAccessView(*ppBuffer, &uavDesc, ppUAV);

	return hr;
}