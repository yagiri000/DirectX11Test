#pragma once
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;


class Transform
{
public:
	Vector3 m_position;
	Vector3 m_scale;
	Quaternion m_rotation;
	Transform();
	Transform(const Vector3& position_, const Vector3& scale_, const Vector3& rotation_);
	~Transform();

	// �ʒu�E�g�嗦�E��]�p�����������s���Ԃ�
	// TODO : ���Ԃ�������������
	Matrix GetMatrix() const;

	// �O�����̃x�N�g����Ԃ�
	// TODO : ���������삷�邩����
	Vector3 Forward() const;
};

