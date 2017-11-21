#pragma once

#include <memory>
#include <vector>
#include "MyVertexTypes.h"
#include "SimpleMath.h"
#include "MinMaxCurve.h"


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

	const UINT m_maxNum;
	UINT m_num;
	float time = 0.0f;
	Matrix m_ViewProj;
	std::vector<Vector3> Positions; // ‰ñ“]‘O‚ÌˆÊ’u
	std::vector<ParticlePoint>		m_particleArray;
	std::unique_ptr<MinMaxCurve4> m_scaleCurve;
	std::unique_ptr<MinMaxCurveRotation> m_rotationCurve;
	std::unique_ptr<MinMaxCurve4> m_colorCurve;;
};

