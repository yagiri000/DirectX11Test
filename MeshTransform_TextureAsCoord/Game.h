//
// Game.h
//

#pragma once

#include "pch.h"
#include "StepTimer.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------

//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
struct SIMPLESHADER_VERTEX_CONSTANT_BUFFER
{
	XMMATRIX mW;
	XMMATRIX mWVP;//ワールド、ビュー、射影の合成変換行列
	XMVECTOR UV;
};

struct SIMPLESHADER_PIXEL_CONSTANT_BUFFER
{
	XMMATRIX mW;
	XMMATRIX mWVP;//ワールド、ビュー、射影の合成変換行列
};

// 平面が原点を常に見るようにするテスト
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
	ComPtr<ID3D11PixelShader>		m_pixelShader;
	ComPtr<ID3D11InputLayout>		m_vertexLayout;
	ComPtr<ID3D11Buffer>			m_vertexConstantBuffer;
	ComPtr<ID3D11Buffer>			m_pixelConstantBuffer;
	std::shared_ptr<ModelMeshPart>	m_model;

	ComPtr<ID3D11ShaderResourceView> pShaderResView;
	ComPtr<ID3D11Resource> pTexture;

	ComPtr<ID3D11ShaderResourceView> pCoordTextureSRV;
	ComPtr<ID3D11Resource> pCoordTexture;

	ComPtr<ID3D11SamplerState> pSampler;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	static constexpr UINT Columns = 36;
	static constexpr UINT Rows = 4;
};