//
// Game.h
//

#pragma once

#include "StepTimer.h"


using namespace DirectX;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
};
	
//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
struct SIMPLESHADER_CONSTANT_BUFFER
{
	XMMATRIX mW;//ワールド、ビュー、射影の合成変換行列
	XMMATRIX mWVP;//ワールド、ビュー、射影の合成変換行列
	XMVECTOR mLightDir;//ワールド、ビュー、射影の合成変換行列
	XMVECTOR mDiffuse;//ワールド、ビュー、射影の合成変換行列
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
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_context;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;
	// Texture Resources
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_sceneTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_sceneSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_sceneRT;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_bloomedTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_bloomedSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_bloomedRT;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_bloomedTex2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_bloomedSRV2;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_bloomedRT2;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>     m_vertexShader = NULL;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>      m_pixelShader = NULL;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>      m_vertexLayout = NULL;
	Microsoft::WRL::ComPtr<ID3D11Buffer>           m_vertexBuffer = NULL;
	Microsoft::WRL::ComPtr<ID3D11Buffer>           m_indexBuffer = NULL;
	Microsoft::WRL::ComPtr<ID3D11Buffer>           m_constantBuffer = NULL;
	UINT					m_indexCount;
	std::unique_ptr<DirectX::BasicPostProcess> m_postProcess;
	std::unique_ptr<DirectX::DualPostProcess> m_dualPostProcess;
	std::unique_ptr<CommonStates> m_commonStates;
    // Rendering loop timer.
    DX::StepTimer                                   m_timer;
};