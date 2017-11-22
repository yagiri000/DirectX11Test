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
		DirectX::XMFLOAT3 Velocity; // 毎フレームPosに加算
		DirectX::XMFLOAT3 Gravity; // 毎フレームVelocityに加算
		DirectX::XMFLOAT3 Scale; // ConstantCurve[Life] (StreachBillboardの際はxが横幅，yが前方幅)
		DirectX::XMFLOAT3 Up; // モデル空間でのビルボードの上方向 : 単位ベクトル
		DirectX::XMFLOAT3 Right; // モデル空間でのビルボードの右方向 : 単位ベクトル
		DirectX::XMFLOAT4 Color; // ConstantCurve[Life]
		DirectX::XMFLOAT4 Life_LifeVel; //xに正規化された生存時間，yに1.0 / 生存時間 毎フレームxにyを加算
	};

	struct ParticleParamaterCash
	{
		static constexpr size_t CashNum = 1024;
		DirectX::XMFLOAT4 Scale[CashNum];
		DirectX::XMFLOAT4 Up[CashNum];
		DirectX::XMFLOAT4 Right[CashNum];
		DirectX::XMFLOAT4 Color[CashNum];
	};

	//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		DirectX::XMMATRIX mW;
		DirectX::XMMATRIX mWVP;//ワールド、ビュー、射影の合成変換行列
		// DirectX::XMVECTOR mCameraDir; // カメラの前方向
	};
};