//グローバル
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
};

struct Particle
{
	float3 Start;
	float3 End;
	float3 Tangent;
};

StructuredBuffer<Particle> ParticlesRO : register(t0);

struct GS_INPUT
{
	float4 Start : SV_POSITION;
	float4 End : NORMAL;
	float4 Tangent : TANGENT;
};

GS_INPUT main(uint ID : SV_VertexID)
{
	GS_INPUT output = (GS_INPUT)0;
	output.Start = float4(ParticlesRO[ID].Start, 1);
	output.End = float4(ParticlesRO[ID].End, 1);
	output.Tangent = float4(ParticlesRO[ID].Tangent, 1);
	return output;
}