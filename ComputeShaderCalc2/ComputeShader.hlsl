

struct BufType
{
	float3 Pos;
	float3 Velocity;
};

RWStructuredBuffer<BufType> BufferOut : register(u0);


[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	//uint num = (DTid.x + 1) % 9999;
	uint num = DTid.x + 1;
	float3 pos = BufferOut[DTid.x].Pos;
	float3 next = BufferOut[num].Pos;
	BufferOut[DTid.x].Velocity = normalize(next - pos) * 0.006 + BufferOut[DTid.x].Velocity;
	BufferOut[DTid.x].Velocity = 0.5 * BufferOut[DTid.x].Velocity;
	BufferOut[DTid.x].Pos = BufferOut[DTid.x].Pos + BufferOut[DTid.x].Velocity;
}