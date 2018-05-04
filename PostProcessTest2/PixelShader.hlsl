
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//グローバル
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
	float4 g_mLife;
};


struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : TEXCOORD0;
	float2 UV : TEXCOORD1;
	float2 Tex : TEXCOORD2;
};

static const float PI = 3.14159265;
static const float PI2 = 3.14159265 * 2;

float4 main(PS_INPUT input) : SV_Target
{
    float4 color = float4(1.0, 1, 1, 1.0);
	return color;
}