#pragma once
#include "Transform.h"

namespace Utility {
	
	// 仮．要修正
	extern Transform CameraTransform;
	void SetCameraTransform(const Transform& t);
	Transform GetCameraTransform();

	// 値をminとmaxの間にして返す
	float Clamp(float value, float min, float max);
}
