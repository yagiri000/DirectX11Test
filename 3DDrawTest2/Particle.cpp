#include "pch.h"
#include "Particle.h"
#include "Utility.h"

using namespace DirectX::SimpleMath;


Transform Particle::GetTransform() const
{
	return Transform();
}

bool Particle::IsDead() const
{
	return m_elapsedTime > m_lifetime;
}

float Particle::GetTimeRate() const
{
	return m_elapsedTime / m_lifetime;
}

void Particle::Update(float deltaTime)
{
	// ���ԍX�V
	m_elapsedTime += deltaTime;

	// �ʒu�X�V
	m_velocity += m_gravity * deltaTime;
	m_position += m_velocity * deltaTime;

	// 
}

void Particle::Draw() const
{
	// TODO : �O���������悤�ɂ���
	Quaternion rotation = Utility::GetCameraTransform().m_rotation;

	float rate = GetTimeRate();
	Transform transform(m_position, (Vector3)m_scaleCurve.Get(rate), rotation);

	Utility::DrawPlane(transform);
}

Particle::Particle()
{
}

Particle::Particle(const Vector3 & position, const Vector3 & velocity, const Vector3 & gravity, const MinMaxCurve4 & scaleCurve, const MinMaxCurveRotation & rotationCurve, const MinMaxCurve4& colorCurve, float lifetime) :
	m_position(position),
	m_velocity(velocity),
	m_gravity(gravity),
	m_scaleCurve(scaleCurve),
	m_rotationCurve(rotationCurve),
	m_colorCurve(m_colorCurve),
	m_lifetime(lifetime),
	m_elapsedTime(0.0f)
{
}


Particle::~Particle()
{
}
