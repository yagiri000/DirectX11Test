//�O���[�o��
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //���[���h�A�r���[�A�ˉe�̍����ϊ��s��
};

struct Particle
{
	float3 Pos;
	float3 Velocity; // ���t���[��Pos�ɉ��Z
	float3 Gravity; // ���t���[��Velocity�ɉ��Z
	float3 Scale; // ConstantCurve[Life] (StreachBillboard�̍ۂ�x�������Cy���O����)
	float3 Up; // ���f����Ԃł̃r���{�[�h�̏���� : �P�ʃx�N�g��
	float3 Right; // ���f����Ԃł̃r���{�[�h�̉E���� : �P�ʃx�N�g��
	float4 Color; // ConstantCurve[Life]
	float4 Life_LifeVel; //x�ɐ��K�����ꂽ�������ԁCy��1.0 / �������� ���t���[��x��y�����Z
};

StructuredBuffer<Particle> ParticlesRO : register(t0);

struct GS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Velocity : VELOCITY;
	float3 Gravity : GRAVITY;
	float3 Scale : SCALE;
	float3 Up : UP;
	float3 Right : RIGHT;
	float4 Color : COLOR;
	float4 Life_LifeVel : LIFE;
};

GS_INPUT main(uint ID : SV_VertexID)
{
	GS_INPUT output = (GS_INPUT)0;
	output.Pos = float4(ParticlesRO[ID].Pos, 1);
	output.Velocity = ParticlesRO[ID].Velocity;
	output.Gravity = ParticlesRO[ID].Gravity;
	output.Scale = ParticlesRO[ID].Scale;
	output.Up = ParticlesRO[ID].Up;
	output.Right = ParticlesRO[ID].Right;
	output.Color = ParticlesRO[ID].Color;
	output.Life_LifeVel = ParticlesRO[ID].Life_LifeVel;
	return output;
}