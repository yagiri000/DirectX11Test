#pragma once

#include "Transform.h"
#include "MinMaxCurve.h"

class Particle
{
public:
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_gravity;
	MinMaxCurve4 m_scaleCurve;
	MinMaxCurveRotation m_rotationCurve;
	MinMaxCurve4 m_colorCurve;

	float m_lifetime;
	float m_elapsedTime;

	Transform GetTransform() const;
	bool IsDead() const;

	// åoâﬂéûä‘ / ê∂ë∂éûä‘Çï‘Ç∑
	float GetTimeRate() const;
	
	void Update(float deltaTime);
	void Draw() const;

	Particle();
	Particle(const Vector3& position, const Vector3& velocity, const Vector3& gravity, const MinMaxCurve4& scaleCurve, const MinMaxCurveRotation& rotationCurve, const MinMaxCurve4& colorCurve, float lifetime);
	~Particle();
};

