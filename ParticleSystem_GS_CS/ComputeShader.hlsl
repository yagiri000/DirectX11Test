//�O���[�o��
cbuffer global : register(b0)
{
	float4 CashedScale[1024];
	float4 CashedRight[1024];
	float4 CashedUp[1024];
	float4 CashedColor[1024];
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

RWStructuredBuffer<Particle> BufferOut : register(u0);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	uint num = DTid.x;
	float3 pos = BufferOut[num].Pos;
	float Life = saturate(BufferOut[num].Life_LifeVel.x);
	BufferOut[num].Velocity += BufferOut[num].Gravity;
	// BufferOut[num].Velocity += float3(sin(pos.y * 20.0), sin(pos.z * 20.0), sin(pos.x * 20.0)) * 0.00003;
	BufferOut[num].Pos += BufferOut[num].Velocity;

	BufferOut[num].Color = CashedColor[(uint)(Life * 1023.0)];
	BufferOut[num].Scale = CashedScale[(uint)(Life * 1023.0)];

	BufferOut[num].Life_LifeVel.x += BufferOut[num].Life_LifeVel.y;
}