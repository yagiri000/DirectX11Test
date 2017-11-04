#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include "pch.h"
#include "Transform.h"

using namespace DirectX;

class Camera
{
public:
	Camera();
	Camera& operator=(const Camera& a) = delete;
	Camera(const Camera& a) = delete;
	static Camera& Get();

	// Šeí‰Šú‰»‚ğs‚¤
	static void Initialize();
	static void SetTransform(const Transform & t);
	static void SetProjectionInfo(float fov, float nearPlane, float farPlane);
	static XMMATRIX View();
	static XMMATRIX Projection();

private:
	Transform m_transform;
	float m_fov;
	float m_nearPlane;
	float m_farPlane;
};