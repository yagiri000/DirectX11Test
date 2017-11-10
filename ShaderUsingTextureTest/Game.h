//
// Game.h
//

#pragma once

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

//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
struct SIMPLESHADER_VERTEX_CONSTANT_BUFFER
{
	XMMATRIX mW;
	XMMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
	XMVECTOR UV;
};

struct SIMPLESHADER_PIXEL_CONSTANT_BUFFER
{
	XMMATRIX mW;
	XMMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
};

// ���ʂ����_����Ɍ���悤�ɂ���e�X�g
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
	ComPtr<ID3D11RasterizerState>	m_rasterizerStateBack;
	ComPtr<ID3D11BlendState>		m_blendState;

	ComPtr<ID3D11VertexShader>		m_vertexShader;
	ComPtr<ID3D11PixelShader>		m_pixelShader;
	ComPtr<ID3D11InputLayout>		m_vertexLayout;
	ComPtr<ID3D11Buffer>			m_vertexBuffer;
	ComPtr<ID3D11Buffer>			m_vertexConstantBuffer;
	ComPtr<ID3D11Buffer>			m_pixelConstantBuffer;

	ComPtr<ID3D11ShaderResourceView> pShaderResView;
	ComPtr<ID3D11SamplerState> pSampler;
	ComPtr<ID3D11Resource> pTexture;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;
};