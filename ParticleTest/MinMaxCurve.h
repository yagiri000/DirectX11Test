#pragma once

#include <SimpleMath.h>
#include <functional>
#include "Easing.h"

using namespace DirectX::SimpleMath;

class MinMaxCurve
{
public:
	float m_start;
	float m_end;
	std::function<float(float, float, float)> m_easing;

	float Get(float rate);
	MinMaxCurve();
	MinMaxCurve(float min, float max);
	MinMaxCurve(float min, float max, const std::function<float(float, float, float)>& easing);
	~MinMaxCurve();
};


class MinMaxCurveRotation
{
public:
	Quaternion m_start;
	Quaternion m_end;
	std::function<float(float, float, float)> m_easing;

	float Get(float rate);
	MinMaxCurveRotation();
	MinMaxCurveRotation(float min, float max);
	MinMaxCurveRotation(float min, float max, const std::function<float(float, float, float)>& easing);
	~MinMaxCurveRotation();
};


// 4つの数を補間して返すクラス 色，拡大率，座標
class MinMaxCurve4
{
public:
	MinMaxCurve m_c0;
	MinMaxCurve m_c1;
	MinMaxCurve m_c2;
	MinMaxCurve m_c3;

	Vector4 Get(float rate);
	MinMaxCurve4(const MinMaxCurve& c0, const MinMaxCurve& c1, const MinMaxCurve& c2, const MinMaxCurve& c3);
};