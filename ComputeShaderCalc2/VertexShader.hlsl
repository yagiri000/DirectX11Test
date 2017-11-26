//グローバル
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
};

struct Particle
{
	float3 Pos;
	float3 Velocity;
};

StructuredBuffer<Particle> ParticlesRO : register(t0);

struct GS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

GS_INPUT main(uint ID : SV_VertexID)
{
	GS_INPUT output = (GS_INPUT)0;
	output.Pos = float4(ParticlesRO[ID].Pos, 1);
	output.Color = float4(1.0, 0.01, 0.01, 0.8);
	return output;
}