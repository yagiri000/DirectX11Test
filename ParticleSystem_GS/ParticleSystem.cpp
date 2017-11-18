#include "pch.h"
#include "ParticleSystem.h"
#include "SimpleMath.h"
#include "Random.h"
#include <string>
#include "Utility.h"
#include "Resource.h"

using namespace DirectX::SimpleMath;


ParticleSystem::ParticleSystem()
{
	for (int i = 0; i < MAXNUM; i++) {
		Positions[i] = Random::OnSphere();
	}
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Update(float deltaTime)
{
	time += deltaTime;

	{
		constexpr int PlusNum = 1000;
		if (GetKeyState('D') & 0x80) {
			m_num += PlusNum;
		}
		if (GetKeyState('A') & 0x80 && m_num > PlusNum) {
			m_num -= PlusNum;
		}

		m_num = Utility::Clamp(m_num, 0, MAXNUM);
	}

	// 行列計算
	Matrix view;
	Matrix proj;

	// ビュートランスフォーム（視点座標変換）
	Vector3 eye(0.0f, 1.0f, 3.0f); //カメラ（視点）位置
	Vector3 lookat(0.0f, 0.0f, 0.0f);//注視位置
	Vector3 up(0.0f, 1.0f, 0.0f);//上方位置
	view = Matrix::CreateLookAt(eye, lookat, up);

	// プロジェクショントランスフォーム（射影変換）
	int width = Resource::DefaultWindowWidth;
	int height = Resource::DefaultWindowWidth;
	proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.0f, (float)width / (float)height, 0.1f, 1000.0f);

	m_ViewProj = view * proj;

	for (int i = 0; i < m_num; i++) {
		Vector3 pos = Vector3::Transform(Positions[i], Matrix::CreateRotationY(time * 0.5f));
		m_particleArray[i].Pos = pos;
	}
}

void ParticleSystem::Render()
{
	// 
	// ---------Rendering-------------
	//
	{
		// コンスタントバッファーに各種データを渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;
		if (SUCCEEDED(m_context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
			auto m = XMMatrixTranspose(m_ViewProj);
			// TODO : ワールドは渡す意味が無いので消す
			cb.mW = Matrix().Transpose();
			cb.mWVP = m;

			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			m_context->Unmap(m_constantBuffer.Get(), 0);
		}
	}


	{
		// コンスタントバッファーに各種データを渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(m_context->Map(m_particles.Get(), 0, D3D11_MAP_WRITE, 0, &pData))) {

			memcpy_s(pData.pData, pData.RowPitch, (void*)(m_particleArray), sizeof(ParticlePoint) * MAXNUM);
			m_context->Unmap(m_particles.Get(), 0);
		}
	}


	// Set primitive topology
	m_context->RSSetState(m_rasterizerState.Get());

	UINT mask = 0xffffffff;
	m_context->OMSetBlendState(m_blendState.Get(), NULL, mask);

	// 使用シェーダー登録
	m_context->VSSetShader(m_vertexShader.Get(), NULL, 0);
	m_context->GSSetShader(m_geometoryShader.Get(), NULL, 0);
	m_context->PSSetShader(m_pixelShader.Get(), NULL, 0);

	// サンプラー
	UINT smp_slot = 0;
	ID3D11SamplerState* smp[1] = { pSampler.Get() };
	m_context->PSSetSamplers(smp_slot, 1, smp);

	// シェーダーリソースビュー（テクスチャ）
	UINT srv_slot = 0;
	ID3D11ShaderResourceView* srv[1] = { pShaderResView.Get() };
	m_context->PSSetShaderResources(srv_slot, 1, srv);

	ID3D11ShaderResourceView* const     g_pNullSRV = NULL;       // Helper to Clear SRVs
	ID3D11UnorderedAccessView* const    g_pNullUAV = NULL;       // Helper to Clear UAVs
	ID3D11Buffer* const                 g_pNullBuffer = NULL;    // Helper to Clear Buffers
	UINT                                g_iNullUINT = 0;         // Helper to Clear Buffers

	m_context->VSSetShaderResources(0, 1, m_particlesSRV.GetAddressOf());
	m_context->IASetVertexBuffers(0, 1, &g_pNullBuffer, &g_iNullUINT, &g_iNullUINT);
	m_context->IASetIndexBuffer(g_pNullBuffer, DXGI_FORMAT_UNKNOWN, 0);
	m_context->IASetInputLayout(nullptr);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	m_context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
	m_context->GSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
	m_context->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

	m_context->Draw(m_num, 0);

	m_context->GSSetShader(nullptr, NULL, 0);

	// Unset the particles buffer
	m_context->VSSetShaderResources(0, 1, &g_pNullSRV);
}

void ParticleSystem::OnInitialize()
{

	m_particleArray = new ParticlePoint[MAXNUM];
	for (UINT i = 0; i < MAXNUM; i++) {
		m_particleArray[i].Pos = Random::OnSphere();
		m_particleArray[i].Normal = Random::OnSphere();
	}

	// Create Structured Buffers
	CreateStructuredBuffer< ParticlePoint >(m_device.Get(), MAXNUM, m_particles.GetAddressOf(), m_particlesSRV.GetAddressOf(), m_particlesUAV.GetAddressOf(), m_particleArray);
}

void ParticleSystem::OnDeviceLost()
{
	pTexture.Reset();
	pShaderResView.Reset();
	pSampler.Reset();

	m_constantBuffer.Reset();
}
