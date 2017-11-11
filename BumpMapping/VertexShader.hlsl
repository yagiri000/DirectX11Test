//�O���[�o��
cbuffer global : register(b0)
{
	matrix g_mW;//���[���h�s��
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_vLightDir;//���C�g�̕����x�N�g��
	float4 g_Diffuse = float4(1, 0, 0, 0); //�g�U����(�F�j	
	float4 g_vEye;	//�J�����i���_�j
};

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float2 Tex : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(float4(input.Pos, 1), g_mWVP);
	output.Tex = input.Tex;
	output.Light = g_vLightDir;

	float3 PosWorld = mul(input.Pos, g_mW);
	output.EyeVector = g_vEye - PosWorld;

	return output;
}