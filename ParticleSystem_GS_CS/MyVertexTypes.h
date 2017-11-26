#pragma once

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------

#include <DirectXMath.h>

namespace DirectX
{
	struct ParticlePoint
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Velocity; // ���t���[��Pos�ɉ��Z
		DirectX::XMFLOAT3 Gravity; // ���t���[��Velocity�ɉ��Z
		DirectX::XMFLOAT3 Scale; // ConstantCurve[Life] (StreachBillboard�̍ۂ�x�������Cy���O����)
		DirectX::XMFLOAT3 Up; // ���f����Ԃł̃r���{�[�h�̏���� : �P�ʃx�N�g��
		DirectX::XMFLOAT3 Right; // ���f����Ԃł̃r���{�[�h�̉E���� : �P�ʃx�N�g��
		DirectX::XMFLOAT4 Color; // ConstantCurve[Life]
		DirectX::XMFLOAT4 Life_LifeVel; //x�ɐ��K�����ꂽ�������ԁCy��1.0 / �������� ���t���[��x��y�����Z
	};

	struct ParticleParamaterCash
	{
		static constexpr size_t CashNum = 1024;
		DirectX::XMFLOAT4 Scale[CashNum];
		DirectX::XMFLOAT4 Up[CashNum];
		DirectX::XMFLOAT4 Right[CashNum];
		DirectX::XMFLOAT4 Color[CashNum];
	};

	//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		DirectX::XMMATRIX mW;
		DirectX::XMMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
		// DirectX::XMVECTOR mCameraDir; // �J�����̑O����
	};
};