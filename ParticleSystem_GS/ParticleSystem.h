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

	static const UINT MAXNUM = 10000;
	UINT m_num = MAXNUM / 2;
	float time = 0.0f;
	Matrix m_ViewProj;
	Vector3 Positions[MAXNUM]; // ‰ñ“]‘O‚ÌˆÊ’u
};

