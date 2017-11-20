//グローバル
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
};

struct Particle
{
	float3 Pos;
	float3 Tangent;
};

StructuredBuffer<Particle> ParticlesRO : register(t0);

struct GS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 NextPos : NORMAL;
	float4 Tangent : TANGENT;
	float4 NextTan : NEXT_TANGENT;
};

GS_INPUT main(uint ID : SV_VertexID)
{
	GS_INPUT output = (GS_INPUT)0;
	output.Pos = float4(ParticlesRO[ID].Pos, 1);
	output.NextPos = float4(ParticlesRO[ID+1].Pos, 1);
	output.Tangent = float4(ParticlesRO[ID].Tangent, 1);
	output.NextTan = float4(ParticlesRO[ID+1].Tangent, 1);
	return output;
}