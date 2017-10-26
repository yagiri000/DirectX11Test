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

	// 位置・拡大率・回転角を結合した行列を返す
	// TODO : 順番が正しいか検証
	Matrix GetMatrix() const;

	// 前方向のベクトルを返す
	// TODO : 正しく動作するか検証
	Vector3 Forward() const;
};

