
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

// uv��Ԃ�(0.5, 0.5)�𒆐S�Ƃ��C���̊p�x�̈�݂̂�`�悷��
// �~�`�̃Q�[�WUI�p

//�O���[�o��
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //���[���h�A�r���[�A�ˉe�̍����ϊ��s��
	float4 g_mAngleStart_Dif; // x�ɊJ�n�p�x�Cy�ɊJ�n�p�x���牽�x�`�悷�邩�������ϐ�
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
	float4 color = txDiffuse.Sample(samLinear, input.Tex);
	
	float2 dir = normalize(float2(input.Tex.x-0.5, input.Tex.y-0.5));

	float startAngle = g_mAngleStart_Dif.x;
	float2 start = float2(cos(startAngle), sin(startAngle));
	float endAngle = g_mAngleStart_Dif.x + g_mAngleStart_Dif.y;
	float2 end = float2(cos(endAngle), sin(endAngle));
	float2 ave = normalize(start + end);

	float angleDif = g_mAngleStart_Dif.y;
	dir = -sign(angleDif - PI) * dir;
	clip(dot(ave, dir) - cos(angleDif*0.5));
	
	return color;
}