#pragma once

#include <memory>
#include "MyVertexTypes.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void Update(float deltaTime);
	void Render(ID3D11Device1* m_device, ID3D11DeviceContext1* m_context);
	void OnInitialize();
	void OnDeviceLost();

	ComPtr<ID3D11Buffer>			m_constantBuffer;

	std::unique_ptr<ParticlePoint[]>		m_particleArray;
	ComPtr<ID3D11Buffer>					m_particles;
	ComPtr<ID3D11ShaderResourceView>		m_particlesSRV;
	ComPtr<ID3D11UnorderedAccessView>		m_particlesUAV;

	static const UINT MAXNUM = 10000;
	UINT m_num = MAXNUM / 2;
	float time = 0.0f;

	Matrix m_ViewProj;

	Vector3 Positions[MAXNUM]; // ‰ñ“]‘O‚ÌˆÊ’u
};

