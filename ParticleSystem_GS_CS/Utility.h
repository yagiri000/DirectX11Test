#pragma once
#include "Transform.h"
#include <functional>

using namespace DirectX;
using namespace DirectX::SimpleMath;


namespace Utility {
	
	// âºÅDóvèCê≥
	extern Transform CameraTransform;
	void SetCameraTransform(const Transform& t);
	Transform GetCameraTransform();

	extern std::function<void(const Transform& trans)> DrawPlane;

	Quaternion LookRotation(const Vector3& dir);


	template<class T>
	T Clamp(T min, T max, T value)
	{
		return std::min(std::max(value, min), max);
	}
}
