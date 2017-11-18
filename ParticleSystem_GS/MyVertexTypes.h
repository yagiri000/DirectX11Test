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

	//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		DirectX::XMMATRIX mW;
		DirectX::XMMATRIX mWVP;//ワールド、ビュー、射影の合成変換行列
	};
};