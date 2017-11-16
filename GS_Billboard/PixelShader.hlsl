
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//�O���[�o��
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //���[���h�A�r���[�A�ˉe�̍����ϊ��s��
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
	float4 color = txDiffuse.Sample(samLinear, input.Tex) * input.Color;
	return color;
}