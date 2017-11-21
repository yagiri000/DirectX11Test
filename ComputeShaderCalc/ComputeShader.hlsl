

struct BufType
{
	float3 Pos;
	float3 Velocity;
};

RWStructuredBuffer<BufType> BufferOut : register(u0);


[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	BufferOut[DTid.x].Pos = BufferOut[DTid.x].Pos + BufferOut[DTid.x].Velocity;
}