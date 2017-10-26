#pragma once

#include <SimpleMath.h>
#include <functional>
#include "Easing.h"

using namespace DirectX::SimpleMath;

// FIXME : �e���v���[�g��������
// FIXME : �N���X��������

class MinMaxCurve
{
public:
	float m_start;
	float m_end;
	std::function<float(float, float, float)> m_easing;

	MinMaxCurve();
	MinMaxCurve(float min, float max);
	MinMaxCurve(float min, float max, const std::function<float(float, float, float)>& easing);
	float Get(float rate) const;
};


class MinMaxCurveRotation
{
public:
	Quaternion m_start;
	Quaternion m_end;
	std::function<float(float, float, float)> m_easing;

	MinMaxCurveRotation();
	MinMaxCurveRotation(const Quaternion& min, const Quaternion& max);
	MinMaxCurveRotation(const Quaternion& min, const Quaternion& max, const std::function<float(float, float, float)>& easing);
	Quaternion Get(float rate) const;
};


// 4�̐����Ԃ��ĕԂ��N���X �F�C�g�嗦�C���W
class MinMaxCurve4
{
public:
	MinMaxCurve m_c0;
	MinMaxCurve m_c1;
	MinMaxCurve m_c2;
	MinMaxCurve m_c3;

	MinMaxCurve4();
	MinMaxCurve4(const MinMaxCurve& c0, const MinMaxCurve& c1, const MinMaxCurve& c2, const MinMaxCurve& c3);
	Vector4 Get(float rate) const;
};