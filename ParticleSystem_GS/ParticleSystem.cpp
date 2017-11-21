#include "pch.h"
#include "ParticleSystem.h"
#include "SimpleMath.h"
#include "Random.h"
#include <string>
#include "Utility.h"
#include "Resource.h"
#include "Input.h"

using namespace DirectX::SimpleMath;


ParticleSystem::ParticleSystem():
	m_maxNum(9999),
	m_num(9999)
{
	Positions.resize(m_maxNum);
	for (int i = 0; i < m_maxNum; i++) {
		Positions[i] = Random::OnSphere();
	}
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Update(float deltaTime)
{
	auto& res = Resource::Get();
	time += deltaTime;

	if (Input::GetKeyDown(Keyboard::Z)) {
		for (int i = 0; i < m_num; i++) {
			auto& p = m_particleArray[i];
			p.Pos = Vector3::Zero;
			auto vel = Random::OnCircle(1.0f);
			p.Velocity = Vector3(vel.x, 0.0f, vel.y) * Random::Range(0.02f, 0.02f);
			p.Gravity = Vector3::Zero;
			p.Scale = Vector3::One * 0.01;
			p.Up = Vector3::Up;
			p.Right = Vector3::Right;
			p.Color = Vector4::One;
			float life = Random::Range(3.5f, 3.5f) * 60.0f;
			p.Life_LifeVel = Vector4(0.0f, 1.0 / life, 0.0f, 0.0f);
		}
	}

	{
		static UINT PlusNum = m_maxNum / 60;
		if (Input::GetKey(Keyboard::A) && m_num > PlusNum) {
			m_num -= PlusNum;
		}
		if (Input::GetKey(Keyboard::D)) {
			m_num += PlusNum;
		}

		m_num = Utility::Clamp(0u, m_maxNum, m_num);
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
	int height = Resource::DefaultWindowHeight;
	proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.0f, (float)width / (float)height, 0.1f, 1000.0f);

	m_ViewProj = view * proj;

	for (int i = 0; i < m_num; i++) {
		auto& p = m_particleArray[i];
		if (p.Life_LifeVel.x > 1.0) {
			continue;
		}
		p.Pos = Vector3(p.Pos) + Vector3(p.Velocity);
		p.Velocity = Vector3(p.Velocity)
			+ Vector3(sin(p.Pos.y * 25.0f), sin(p.Pos.z * 25.0f), sin(p.Pos.x * 25.0f)) * 0.0002f * pow(Vector3(p.Pos).Length(), 0.3);
		p.Velocity = Vector3(p.Velocity) * 0.99f;
		p.Velocity = Vector3(p.Velocity) +  Vector3(p.Gravity);
		p.Scale = Vector3(m_scaleCurve->Get(p.Life_LifeVel.x));
		p.Up = Vector3::Up;
		p.Right = Vector3::Right;
		p.Color = m_colorCurve->Get(p.Life_LifeVel.x);
		p.Life_LifeVel.x += p.Life_LifeVel.y;
	}
}

void ParticleSystem::Render()
{
	auto& res = Resource::Get();

	{
		// コンスタントバッファーに各種データを渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;
		if (SUCCEEDED(res.m_context->Map(res.m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
			auto m = XMMatrixTranspose(m_ViewProj);
			cb.mW = Matrix().Transpose();
			cb.mWVP = m;
			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			res.m_context->Unmap(res.m_constantBuffer.Get(), 0);
		}
	}


	{
		// StructuredBufferにパーティクルデータを受け渡し
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(res.m_context->Map(res.m_particles.Get(), 0, D3D11_MAP_WRITE, 0, &pData))) {
			memcpy_s(pData.pData, pData.RowPitch, (void*)(&m_particleArray[0]), sizeof(ParticlePoint) * m_maxNum);
			res.m_context->Unmap(res.m_particles.Get(), 0);
		}
	}


	// Set primitive topology
	res.m_context->RSSetState(res.m_rasterizerState.Get());

	UINT mask = 0xffffffff;
	res.m_context->OMSetBlendState(res.m_blendState.Get(), NULL, mask);

	// 使用シェーダー登録
	res.m_context->VSSetShader(res.m_vertexShader.Get(), NULL, 0);
	res.m_context->GSSetShader(res.m_geometoryShader.Get(), NULL, 0);
	res.m_context->PSSetShader(res.m_pixelShader.Get(), NULL, 0);

	// サンプラー
	UINT smp_slot = 0;
	ID3D11SamplerState* smp[1] = { res.pSampler.Get() };
	res.m_context->PSSetSamplers(smp_slot, 1, smp);

	// シェーダーリソースビュー（テクスチャ）
	UINT srv_slot = 0;
	ID3D11ShaderResourceView* srv[1] = { res.pShaderResView.Get() };
	res.m_context->PSSetShaderResources(srv_slot, 1, srv);

	ID3D11ShaderResourceView* const     g_pNullSRV = NULL;       // Helper to Clear SRVs
	ID3D11UnorderedAccessView* const    g_pNullUAV = NULL;       // Helper to Clear UAVs
	ID3D11Buffer* const                 g_pNullBuffer = NULL;    // Helper to Clear Buffers
	UINT                                g_iNullUINT = 0;         // Helper to Clear Buffers

	res.m_context->VSSetShaderResources(0, 1, res.m_particlesSRV.GetAddressOf());
	res.m_context->IASetVertexBuffers(0, 1, &g_pNullBuffer, &g_iNullUINT, &g_iNullUINT);
	res.m_context->IASetIndexBuffer(g_pNullBuffer, DXGI_FORMAT_UNKNOWN, 0);
	res.m_context->IASetInputLayout(nullptr);
	res.m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	res.m_context->VSSetConstantBuffers(0, 1, res.m_constantBuffer.GetAddressOf());
	res.m_context->GSSetConstantBuffers(0, 1, res.m_constantBuffer.GetAddressOf());
	res.m_context->PSSetConstantBuffers(0, 1, res.m_constantBuffer.GetAddressOf());

	res.m_context->Draw(m_num, 0);

	res.m_context->GSSetShader(nullptr, NULL, 0);

	// Unset the particles buffer
	res.m_context->VSSetShaderResources(0, 1, &g_pNullSRV);
}

void ParticleSystem::OnInitialize()
{
	m_particleArray.resize(m_maxNum);

	for (int i = 0; i < m_maxNum; i++) {
		m_particleArray[i].Pos = Vector3::Zero;
		m_particleArray[i].Velocity = Random::OnSphere();
		m_particleArray[i].Gravity = Vector3::Zero;
		m_particleArray[i].Scale = Vector3::One;
		m_particleArray[i].Up = Vector3::Up;
		m_particleArray[i].Right = Vector3::Right;
		m_particleArray[i].Color = Vector4::One; 
		m_particleArray[i].Life_LifeVel = Vector4(0.0f, 0.01f, 0.0f, 0.0f); 
	}
	m_scaleCurve = std::make_unique<MinMaxCurve4>(
		MinMaxCurve(0.05f, 0.00f),
		MinMaxCurve(0.05f, 0.00f),
		MinMaxCurve(0.05f, 0.00f),
		MinMaxCurve(0.05f, 0.00f)
		);
	m_rotationCurve = std::make_unique<MinMaxCurveRotation>(
		Quaternion()
	);
	m_colorCurve = std::make_unique<MinMaxCurve4>(
		MinMaxCurve(Random::Value(), 1.0f),
		MinMaxCurve(Random::Range(0.2f, 0.0f), 0.0f),
		MinMaxCurve(Random::Range(0.5f, 0.0f), 0.0f),
		MinMaxCurve(1.0f, 1.0f)
		);
}

void ParticleSystem::OnDeviceLost()
{
}
