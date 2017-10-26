#include "pch.h"
#include "MinMaxCurve.h"



float MinMaxCurve::Get(float rate)
{
	return m_easing(m_start, m_end, rate);
}

MinMaxCurve::MinMaxCurve() :
	m_start(0.0f),
	m_end(0.0f),
	m_easing(Easing::Linear)
{
}

MinMaxCurve::MinMaxCurve(float min, float max) :
	m_start(min),
	m_end(max),
	m_easing(Easing::Linear)
{
}


MinMaxCurve::MinMaxCurve(float min, float max, const std::function<float(float, float, float)>& easing) :
	m_start(min),
	m_end(max),
	m_easing(easing)
{
}

MinMaxCurve::~MinMaxCurve()
{
}
