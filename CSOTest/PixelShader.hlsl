//�O���[�o��
cbuffer global
{
	matrix g_mWVP; //���[���h�A�r���[�A�ˉe�̍����ϊ��s��
};

float4 main() : SV_TARGET
{
	return float4(1.0f, 0.5f, 0.0f, 1.0f);
}