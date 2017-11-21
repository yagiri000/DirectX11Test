#include "pch.h"
#include "Utility.h"
#include <DirectXMath.h>

// TODO : Clamp�̕ϐ���(_2, _0, _1)�̏��ԂɕύX

namespace Utility
{

	Transform CameraTransform;

	// ���t���[���Ă�
	void SetCameraTransform(const Transform& t)
	{
		CameraTransform = t;
	}

	Transform GetCameraTransform()
	{
		return CameraTransform;
	}

	// TODO : ������������
	Quaternion LookRotation(const Vector3 & dir)
	{
		XMMATRIX view = XMMatrixLookToRH(XMVectorZero(), XMVectorSet(dir.x, dir.y, dir.z, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		XMVECTOR quaternion = XMQuaternionRotationMatrix(view);
		// �r���[�s��Ȃ̂�Inverse����
		quaternion = XMQuaternionInverse(quaternion);
		return Quaternion(quaternion);
	}


	std::function<void(const Transform& trans)> DrawPlane;

}
