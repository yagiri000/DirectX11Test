#pragma once
#include "Transform.h"
#include <functional>

using namespace DirectX;
using namespace DirectX::SimpleMath;


namespace Utility {
	
	// ‰¼D—vC³
	extern Transform CameraTransform;
	void SetCameraTransform(const Transform& t);
	Transform GetCameraTransform();

	// ’l‚ğmin‚Æmax‚ÌŠÔ‚É‚µ‚Ä•Ô‚·
	float Clamp(float value, float min, float max);

	extern std::function<void(const Transform& trans, const Vector4& color)> DrawPlane;

	Quaternion LookRotation(const Vector3& dir);
}
