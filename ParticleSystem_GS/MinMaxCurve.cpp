#include "pch.h"
#include "MinMaxCurve.h"
#include "Utility.h"


constexpr size_t MinMaxCurve::CASH_NUM;
constexpr size_t MinMaxCurveRotation::CASH_NUM;

// FIXME : Clampä÷êîÇÇ±Ç±Ç≈égÇ§Ç©

MinMaxCurve::MinMaxCurve() :
	m_start(0.0f),
	m_end(0.0f),
	m_easing(Easing::Linear)
{
	CalcCash();
}

MinMaxCurve::MinMaxCurve(float start, float end) :
	m_start(start),
	m_end(end),
	m_easing(Easing::Linear)
{
	CalcCash();
}


MinMaxCurve::MinMaxCurve(float start, float end, const std::function<float(float, float, float)>& easing) :
	m_start(start),
	m_end(end),
	m_easing(easing)
{
	CalcCash();
}


float MinMaxCurve::Get(float rate) const
{
	size_t n = Utility::Clamp((size_t)(rate * CASH_NUM), (size_t)0, (size_t)CASH_NUM-1);
	return m_cash[n];
	//return m_easing(m_start, m_end, rate);
}

void MinMaxCurve::CalcCash()
{
	for (int i = 0; i < CASH_NUM; i++) {
		m_cash[i] = m_easing(m_start, m_end, (float)i / (float)CASH_NUM);
	}
}


MinMaxCurveRotation::MinMaxCurveRotation() :
	m_start(),
	m_end(),
	m_easing(Easing::Linear)
{
	CalcCash();
}

MinMaxCurveRotation::MinMaxCurveRotation(const Quaternion & rot) :
	m_start(rot),
	m_end(rot),
	m_easing(Easing::Linear)
{
	CalcCash();
}

MinMaxCurveRotation::MinMaxCurveRotation(const Quaternion & start, const Quaternion & end):
	m_start(start),
	m_end(end),
	m_easing(Easing::Linear)
{
	CalcCash();
}

MinMaxCurveRotation::MinMaxCurveRotation(const Quaternion & start, const Quaternion & end, const std::function<float(float, float, float)>& easing):
	m_start(start),
	m_end(end),
	m_easing(easing)
{
	CalcCash();
}

Quaternion MinMaxCurveRotation::Get(float rate) const
{
	size_t n = Utility::Clamp((size_t)(rate * CASH_NUM), (size_t)0, (size_t)CASH_NUM - 1);
	return m_cash[n];
	//rate = Utility::Clamp(rate, 0.0f, 1.0f);
	//return Quaternion::Slerp(m_start, m_end, m_easing(0.0f, 1.0f, rate));
}

void MinMaxCurveRotation::CalcCash()
{
	for (int i = 0; i < CASH_NUM; i++) {
		m_cash[i] = Quaternion::Slerp(m_start, m_end, m_easing(0.0f, 1.0f, (float)i / CASH_NUM));
	}
}



MinMaxCurve4::MinMaxCurve4()
{
}

MinMaxCurve4::MinMaxCurve4(const MinMaxCurve & c0, const MinMaxCurve & c1, const MinMaxCurve & c2, const MinMaxCurve & c3):
	m_c0(c0),
	m_c1(c1),
	m_c2(c2),
	m_c3(c3)
{
}

Vector4 MinMaxCurve4::Get(float rate) const
{
	rate = Utility::Clamp(rate, 0.0f, 1.0f);
	return Vector4(
		m_c0.Get(rate),
		m_c1.Get(rate),
		m_c2.Get(rate),
		m_c3.Get(rate)
	);;
}