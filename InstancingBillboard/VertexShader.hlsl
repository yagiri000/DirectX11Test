//�O���[�o��
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //���[���h�A�r���[�A�ˉe�̍����ϊ��s��
};

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 InstancePos : INSTANCEPOS;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	float3 pos = input.Pos * 0.01 + input.InstancePos;
	output.Pos = mul(float4(pos,1), g_mWVP);
	output.Normal = normalize(mul(input.Normal, (float3x3)g_mW));
	output.Tex = input.Tex;
	return output;
}