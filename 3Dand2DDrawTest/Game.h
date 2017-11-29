//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "CommonStates.h"

using Microsoft::WRL::ComPtr;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
};

// 3DポリゴンとSpriteBatchを用いて2Dテクスチャを描画する例
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
	void GetDefaultSize(int& width, int& height) const;

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

	ComPtr<ID3D11VertexShader>		m_vertexShader;
	ComPtr<ID3D11PixelShader>		m_pixelShader;
	ComPtr<ID3D11InputLayout>		m_vertexLayout;
	ComPtr<ID3D11Buffer>			m_vertexBuffer;
	std::unique_ptr<DirectX::CommonStates> m_states;

	ComPtr<ID3D11ShaderResourceView> m_texture;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	DirectX::SimpleMath::Vector2 m_pos;

	// Rendering loop timer.
	DX::StepTimer                                   m_timer;
};