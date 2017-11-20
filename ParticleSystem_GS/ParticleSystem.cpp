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

	{
		int PlusNum = m_maxNum / 30;
		if (GetKeyState('D') & 0x80) {
			m_num += PlusNum;
		}
		if (GetKeyState('A') & 0x80 && m_num > PlusNum) {
			m_num -= PlusNum;
		}

		m_num = Utility::Clamp(m_num, 0, m_maxNum);
	}

	// �s��v�Z
	Matrix view;
	Matrix proj;

	// �r���[�g�����X�t�H�[���i���_���W�ϊ��j
	Vector3 eye(0.0f, 1.0f, 3.0f); //�J�����i���_�j�ʒu
	Vector3 lookat(0.0f, 0.0f, 0.0f);//�����ʒu
	Vector3 up(0.0f, 1.0f, 0.0f);//����ʒu
	view = Matrix::CreateLookAt(eye, lookat, up);

	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	int width = Resource::DefaultWindowWidth;
	int height = Resource::DefaultWindowHeight;
	proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.0f, (float)width / (float)height, 0.1f, 1000.0f);

	m_ViewProj = view * proj;

	for (int i = 0; i < m_num; i++) {
		Vector3 pos = Vector3::Transform(Positions[i], Matrix::CreateRotationY(time * 0.5f));
		m_particleArray[i].Pos = pos;
	}
}

void ParticleSystem::Render()
{
	auto& res = Resource::Get();

	{
		// �R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;
		if (SUCCEEDED(res.m_context->Map(res.m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
			auto m = XMMatrixTranspose(m_ViewProj);
			// TODO : ���[���h�͓n���Ӗ��������̂ŏ���
			cb.mW = Matrix().Transpose();
			cb.mWVP = m;

			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			res.m_context->Unmap(res.m_constantBuffer.Get(), 0);
		}
	}


	{
		// �R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
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

	// �g�p�V�F�[�_�[�o�^
	res.m_context->VSSetShader(res.m_vertexShader.Get(), NULL, 0);
	res.m_context->GSSetShader(res.m_geometoryShader.Get(), NULL, 0);
	res.m_context->PSSetShader(res.m_pixelShader.Get(), NULL, 0);

	// �T���v���[
	UINT smp_slot = 0;
	ID3D11SamplerState* smp[1] = { res.pSampler.Get() };
	res.m_context->PSSetSamplers(smp_slot, 1, smp);

	// �V�F�[�_�[���\�[�X�r���[�i�e�N�X�`���j
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
}

void ParticleSystem::OnDeviceLost()
{
}
