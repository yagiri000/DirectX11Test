
//�O���[�o��
cbuffer global
{
	matrix g_mWVP; //���[���h�A�r���[�A�ˉe�̍����ϊ��s��
};
//
//�o�[�e�b�N�X�V�F�[�_�[
//
float4 VS(float4 Pos : POSITION) : SV_POSITION
{
	Pos = mul(Pos,g_mWVP);
	return Pos;
}
//
//�s�N�Z���V�F�[�_�[
//
float4 PS(float4 Pos : SV_POSITION) : SV_Target
{
	float4 color = float4(0,1,0,0);
	return color;
}