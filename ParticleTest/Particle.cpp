#include "pch.h"
#include "Particle.h"


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
	m_elapsedTime += deltaTime;
	m_position += m_velocity * deltaTime;
	m_velocity += m_gravity;
}

void Particle::Draw() const
{
}

Particle::Particle() :

{
}


Particle::~Particle()
{
}
