
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//�O���[�o��
cbuffer global : register(b0)
{
	matrix g_mW;//���[���h�s��
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_vLightDir;//���C�g�̕����x�N�g��
	float4 g_Diffuse = float4(1, 0, 0, 0); //�g�U����(�F�j	
	float4 g_vEye;	//�J�����i���_�j
};


struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float2 Tex : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
};

float4 main(PS_INPUT input) : SV_Target
{
	float3 Normal = txDiffuse.Sample(samLinear, input.Tex);
	Normal = 2.0 * Normal + float3(-1.0, -1.0, -1.0);
	Normal = normalize(Normal);

	Normal = mul(Normal, (float3x3)g_mW);
	float3 LightDir = normalize(input.Light);
	float3 ViewDir = normalize(input.EyeVector);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = 2 * pow(saturate(dot(Reflect, ViewDir)), 3);


	float4 color = g_Diffuse * NL + specular;
	color.w = 1.0;
	return color;
}