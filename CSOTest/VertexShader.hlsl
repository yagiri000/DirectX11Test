//�O���[�o��
cbuffer global
{
	matrix g_mWVP; //���[���h�A�r���[�A�ˉe�̍����ϊ��s��
};

float4 main(float4 pos : POSITION) : SV_POSITION
{
	pos = mul(pos,g_mWVP);
	return pos;
}