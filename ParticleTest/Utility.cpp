#include "pch.h"
#include "Utility.h"

namespace Utility {
	
	Transform CameraTransform;

	// ���t���[���Ă�
	void SetCameraTransform(const Transform& t){
		CameraTransform = t;
	}

	Transform GetCameraTransform()
	{
		return CameraTransform;
	}

	float Clamp(float value, float min, float max)
	{
		return std::min(std::max(value, min), max);
	}
}
