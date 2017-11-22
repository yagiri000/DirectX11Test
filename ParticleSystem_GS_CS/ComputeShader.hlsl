struct Particle
{
	float3 Pos;
	float3 Velocity; // 毎フレームPosに加算
	float3 Gravity; // 毎フレームVelocityに加算
	float3 Scale; // ConstantCurve[Life] (StreachBillboardの際はxが横幅，yが前方幅)
	float3 Up; // モデル空間でのビルボードの上方向 : 単位ベクトル
	float3 Right; // モデル空間でのビルボードの右方向 : 単位ベクトル
	float4 Color; // ConstantCurve[Life]
	float4 Life_LifeVel; //xに正規化された生存時間，yに1.0 / 生存時間 毎フレームxにyを加算
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