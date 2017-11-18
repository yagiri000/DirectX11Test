//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "MyVertexTypes.h"
#include "ParticleSystem.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;


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

class Game
{
public:
    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();


	std::unique_ptr<ParticleSystem> m_particleSystem;


    // Rendering loop timer.
    DX::StepTimer                                   m_timer;
};