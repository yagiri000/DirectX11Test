//グローバル
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
};

struct Particle
{
	float3 Pos;
	float3 Velocity; // 毎フレームPosに加算
	float3 Gravity; // 毎フレームVelocityに加算
	float3 Scale; // ConstantCurve[Life] (StreachBillboardの際はxが横幅，yが前方幅)
	float3 Up; // モデル空間でのビルボードの上方向 : 単位ベクトル
	float3 Right; // モデル空間でのビルボードの右方向 : 単位ベクトル
	float4 Color; // ConstantCurve[Life]
	float4 Life_LifeVel; //xに正規化された生存時間，yに1.0 / 生存時間 毎フレームxにyを加算
};

StructuredBuffer<Particle> ParticlesRO : register(t0);

struct GS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Velocity : VELOCITY;
	float3 Gravity : GRAVITY;
	float3 Scale : SCALE;
	float3 Up : UP;
	float3 Right : RIGHT;
	float4 Color : COLOR;
	float4 Life_LifeVel : LIFE;
};

GS_INPUT main(uint ID : SV_VertexID)
{
	GS_INPUT output = (GS_INPUT)0;
	output.Pos = float4(ParticlesRO[ID].Pos, 1);
	output.Velocity = ParticlesRO[ID].Velocity;
	output.Gravity = ParticlesRO[ID].Gravity;
	output.Scale = ParticlesRO[ID].Scale;
	output.Up = ParticlesRO[ID].Up;
	output.Right = ParticlesRO[ID].Right;
	output.Color = ParticlesRO[ID].Color;
	output.Life_LifeVel = ParticlesRO[ID].Life_LifeVel;
	return output;
}