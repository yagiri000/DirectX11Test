#pragma once

#include "Transform.h"
#include "MinMaxCurve.h"

class Particle
{
public:
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_gravity;
	MinMaxCurve4 m_size;
	MinMaxCurveRotation m_rotation;

	float m_lifetime;
	float m_elapsedTime;

	Transform GetTransform() const;
	bool IsDead() const;

	// åoâﬂéûä‘ / ê∂ë∂éûä‘Çï‘Ç∑
	float GetTimeRate() const;
	
	void Update(float deltaTime);
	void Draw() const;

	Particle();
	~Particle();
};

