#pragma once
#include "Transform.h"

namespace Utility {
	
	// ���D�v�C��
	extern Transform CameraTransform;
	void SetCameraTransform(const Transform& t);
	Transform GetCameraTransform();

	// �l��min��max�̊Ԃɂ��ĕԂ�
	float Clamp(float value, float min, float max);
}
