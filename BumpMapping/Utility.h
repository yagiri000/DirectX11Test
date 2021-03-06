#pragma once
#include "Transform.h"
#include <functional>

using namespace DirectX;
using namespace DirectX::SimpleMath;


namespace Utility {
	
	// 仮．要修正
	extern Transform CameraTransform;
	void SetCameraTransform(const Transform& t);
	Transform GetCameraTransform();

	// 値をminとmaxの間にして返す
	float Clamp(float value, float min, float max);

	extern std::function<void(const Transform& trans, const Vector4& color)> DrawPlane;

	Quaternion LookRotation(const Vector3& dir);
}
