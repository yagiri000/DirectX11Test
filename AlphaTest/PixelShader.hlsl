
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//�O���[�o��
cbuffer global : register(b0)
{
	matrix g_mW;
	matrix g_mWVP; //���[���h�A�r���[�A�ˉe�̍����ϊ��s��
	float4 g_UVScroll;
	float4 g_AlphaThreshold;
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
	float4 color = txDiffuse.Sample(samLinear, input.UV) * input.Color;
	clip(color.w - g_AlphaThreshold.x);
	//color = float4(1.0f, 1.0f, 1.0f, 1.0);
	color.w = 1.0;
	return color;
}