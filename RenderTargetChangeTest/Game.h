//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "CommonStates.h"
#include "Keyboard.h"
#include "Mouse.h"

using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Vector2;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
};

// マウスの座標に応じてテクスチャの位置を変化させる例
// 三角形ポリゴンも描画しているが，特に意味はない
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
	ComPtr<ID3D11Device1>           m_d3dDevice;
	ComPtr<ID3D11DeviceContext1>    m_d3dContext;

	ComPtr<IDXGISwapChain1>         m_swapChain;
	ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
	ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

	ComPtr<ID3D11VertexShader>		m_vertexShader;
	ComPtr<ID3D11PixelShader>		m_pixelShader;
	ComPtr<ID3D11InputLayout>		m_vertexLayout;
	ComPtr<ID3D11Buffer>			m_vertexBuffer;

	ComPtr<ID3D11ShaderResourceView> m_texture;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	DirectX::SimpleMath::Vector2 m_pos;
	std::unique_ptr<DirectX::CommonStates> m_states;

	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;

	// Rendering loop timer.
	DX::StepTimer                                   m_timer;
};