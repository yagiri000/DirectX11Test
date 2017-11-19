
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//グローバル
cbuffer global : register(b0)
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
	float4 g_UVScroll;
	float4 g_Life;
};


struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 UV : TEXCOORD1;
	float2 Tex : TEXCOORD2;
};

float4 main(PS_INPUT input) : SV_Target
{
	//float4 color = txDiffuse.Sample(samLinear, input.UV) * input.Color;
	float4 color = input.Color;
	// color.w = input.Tex.x * input.Tex.x * color.w;
	color = float4(1.0f, 1.0f, 1.0f, 1.0 - g_Life.x);
	return color;
}