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
	BufferOut[num].Pos += BufferOut[num].Velocity;
	BufferOut[num].Velocity += BufferOut[num].Gravity;
	BufferOut[num].Life_LifeVel.x += BufferOut[num].Life_LifeVel.y;
}