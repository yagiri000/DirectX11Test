#pragma once

#include <memory>
#include <vector>
#include "MyVertexTypes.h"
#include "SimpleMath.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();
	void Update(float deltaTime);
	void Render();
	void OnInitialize();
	void OnDeviceLost();

	const UINT m_maxNum = 1000;
	UINT m_num = m_maxNum / 2;
	float time = 0.0f;
	Matrix m_ViewProj;
	std::vector<Vector3> Positions; // ‰ñ“]‘O‚ÌˆÊ’u
	std::vector<ParticlePoint>		m_particleArray;
};

