//
// Game.h
//

#pragma once

#include <vector>
#include <memory>
#include "StepTimer.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 UV;
};

// VertexShader用コンスタントバッファー
struct SIMPLESHADER_CONSTANT_BUFFER
{
	XMMATRIX mW;
	XMMATRIX mWVP;//ワールド、ビュー、射影の合成変換行列
	XMVECTOR mLightDir;
	XMVECTOR mDiffuse;
	XMVECTOR mEye;
};

// PixelShader用コンスタントバッファー
struct PIXELSHADER_CONSTANT_BUFFER
{
	XMMATRIX mW;
	XMMATRIX mWVP;//ワールド、ビュー、射影の合成変換行列
	XMVECTOR mLightDir;
	XMVECTOR mDiffuse;
	XMVECTOR mEye;
};

// 三角形ポリゴンを描画する例
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

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

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
	ComPtr<ID3D11BlendState>		m_blendState;

	ComPtr<ID3D11VertexShader>		m_vertexShader;
	ComPtr<ID3D11PixelShader>		m_pixelShader;
	ComPtr<ID3D11InputLayout>		m_vertexLayout;
	ComPtr<ID3D11Buffer>			m_vertexBuffer;
	ComPtr<ID3D11Buffer>			m_indexBuffer;
	ComPtr<ID3D11Buffer>			m_constantBuffer;
	ComPtr<ID3D11Buffer>			m_constantBufferPixel;

	ComPtr<ID3D11ShaderResourceView> pShaderResView;
	ComPtr<ID3D11SamplerState> pSampler;
	ComPtr<ID3D11Resource> pTexture;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;
};