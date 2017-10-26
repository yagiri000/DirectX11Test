#include "pch.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;

Transform::Transform():
	m_position(),
	m_scale(),
	m_rotation()
{
}

Transform::Transform(const Vector3 & position_, const Vector3 & scale_, const Vector3 & rotation_):
	m_position(position_),
	m_scale(scale_),
	m_rotation(rotation_)
{
}


Transform::~Transform()
{
}

Matrix Transform::GetMatrix() const
{
	return Matrix::CreateScale(m_scale) * Matrix::CreateFromQuaternion(m_rotation) * Matrix::CreateTranslation(m_position);
}

Vector3 Transform::Forward() const
{
	return Vector3::Transform(Vector3::Forward, Matrix::CreateFromQuaternion(m_rotation));
}
