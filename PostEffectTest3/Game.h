//
// Game.h
//

#pragma once

#include "StepTimer.h"


// png‰æ‘œ‚ð•`‰æ‚·‚é—á
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
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_context;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	// Texture Resources
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_sceneTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_sceneSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_sceneRT;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_bloomedTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_bloomedSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_bloomedRT;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	DirectX::SimpleMath::Vector2 m_pos;


	std::unique_ptr<DirectX::BasicPostProcess> m_postProcess;
	std::unique_ptr<DirectX::DualPostProcess> m_dualPostProcess;


};