#include "pch.h"
#include "Camera.h"
#include "Transform.h"

using namespace DirectX;

Camera::Camera()
{
}

Camera & Camera::Get()
{
	static Camera r;
	return r;
}

void Camera::Initialize()
{
}

void Camera::SetTransform(const Transform& t)
{
	Camera& ins = Get();
	ins.m_transform = t;
}

void Camera::SetProjectionInfo(float fov, float nearPlane, float farPlane)
{
	Camera& ins = Get();
	ins.m_fov = fov;
	ins.m_nearPlane = nearPlane;
	ins.m_farPlane = farPlane;
}

XMMATRIX Camera::View()
{
	Camera& ins = Get();
	auto rotation = ins.m_transform.m_rotation;
	rotation.w *= -1.0f;
	return Matrix::CreateTranslation(-ins.m_transform.m_position) * Matrix::CreateFromQuaternion(rotation);
}

XMMATRIX Camera::Projection()
{
	Camera& ins = Get();
	return Matrix::CreatePerspectiveFieldOfView(ins.m_fov, (float)800 / (float)600, ins.m_nearPlane, ins.m_farPlane);
}
