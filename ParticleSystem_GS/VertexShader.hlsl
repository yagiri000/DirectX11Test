//グローバル
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
};

struct Particle
{
	float3 Pos;
	float3 Normal;
};

StructuredBuffer<Particle> ParticlesRO : register(t0);

struct GS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : TEXCOORD0;
};

GS_INPUT main(uint ID : SV_VertexID)
{
	GS_INPUT output = (GS_INPUT)0;
	output.Pos = float4(ParticlesRO[ID].Pos, 1);
	output.Normal = normalize(mul(ParticlesRO[ID].Normal, (float3x3)g_mW));
	return output;
}