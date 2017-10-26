#pragma once
#include "Transform.h"

namespace Utility {
	
	// ‰¼D—vC³
	extern Transform CameraTransform;
	void SetCameraTransform(const Transform& t);
	Transform GetCameraTransform();

	// ’l‚ğmin‚Æmax‚ÌŠÔ‚É‚µ‚Ä•Ô‚·
	float Clamp(float value, float min, float max);
}
