#include "pch.h"
#include "MinMaxCurve.h"
#include "Utility.h"

// FIXME : ClampŠÖ”‚ğ‚±‚±‚Åg‚¤‚©

MinMaxCurve::MinMaxCurve() :
	m_start(0.0f),
	m_end(0.0f),
	m_easing(Easing::Linear)
{
}

MinMaxCurve::MinMaxCurve(float start, float end) :
	m_start(start),
	m_end(end),
	m_easing(Easing::Linear)
{
}


MinMaxCurve::MinMaxCurve(float start, float end, const std::function<float(float, float, float)>& easing) :
	m_start(start),
	m_end(end),
	m_easing(easing)
{
}


float MinMaxCurve::Get(float rate) const
{
	rate = Utility::Clamp(rate, 0.0f, 1.0f);
	return m_easing(m_start, m_end, rate);
}


MinMaxCurveRotation::MinMaxCurveRotation() :
	m_start(),
	m_end(),
	m_easing(Easing::Linear)
{
}

MinMaxCurveRotation::MinMaxCurveRotation(const Quaternion & start, const Quaternion & end):
	m_start(start),
	m_end(end),
	m_easing(Easing::Linear)
{
}

MinMaxCurveRotation::MinMaxCurveRotation(const Quaternion & start, const Quaternion & end, const std::function<float(float, float, float)>& easing):
	m_start(start),
	m_end(end),
	m_easing(easing)
{
}

Quaternion MinMaxCurveRotation::Get(float rate) const
{
	rate = Utility::Clamp(rate, 0.0f, 1.0f);
	return Quaternion::Slerp(m_start, m_end, m_easing(0.0f, 1.0f, rate));
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