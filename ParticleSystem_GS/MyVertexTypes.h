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
		DirectX::XMFLOAT3 Normal;
	};

	//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		DirectX::XMMATRIX mW;
		DirectX::XMMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
	};
};