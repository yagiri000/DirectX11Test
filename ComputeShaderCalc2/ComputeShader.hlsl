

struct BufType
{
	float3 Pos;
	float3 Velocity;
};

RWStructuredBuffer<BufType> BufferOut : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	//uint num = (DTid.x + 1) % 9999;
	uint num = DTid.x * 64 * 64 + DTid.y * 64 + DTid.z;
	uint nextNum = (num + 1) % (64 * 64 * 64);
	float3 pos = BufferOut[num].Pos;
	float3 next = BufferOut[nextNum].Pos;
	BufferOut[num].Velocity = normalize(next - pos) * 0.008 + BufferOut[num].Velocity;
	BufferOut[num].Velocity = 0.5 * BufferOut[num].Velocity;
	BufferOut[num].Pos = BufferOut[num].Pos + BufferOut[num].Velocity;
}