//�O���[�o��
cbuffer global
{
	matrix g_mWVP; //���[���h�A�r���[�A�ˉe�̍����ϊ��s��
	float4 alpha;
};

//�\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : TEXCOORD1; // NORMAL�ł�TEXCOORD�ł��Ȃ�
};


VS_OUTPUT main(float4 pos : POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	pos.w = 1;
	output.Pos = mul(pos, g_mWVP);
	output.Normal = pos;
	return output;
}