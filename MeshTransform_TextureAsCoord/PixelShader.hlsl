
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//グローバル
cbuffer global : register(b0)
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_Target
{
	//float4 color = txDiffuse.Sample(samLinear, input.UV) * input.Color;
	float4 color = txDiffuse.Sample(samLinear, input.Tex);
	//color = float4(1.0, 0.0, 0.0, 1.0);
	return color;
}