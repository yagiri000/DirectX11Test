//グローバル
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
};

struct GS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 NextPos : NORMAL;
	float4 Tangent : TANGENT;
	float4 NextTan : NEXT_TANGENT;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};

static const float2 g_posFlag[4] = { float2(1, 0), float2(1, 0), float2(0, 1), float2(0, 1) };
static const float2 g_tanFlag[4] = { float2(1, 0), float2(-1, 0), float2(0, 1), float2(0, -1) };
static const float2 g_texcoords[4] = { float2(1, 0), float2(1, 1), float2(0, 0), float2(0, 1) };
static const float width = 0.05;

[maxvertexcount(4)]
void main(point GS_INPUT In[1], inout TriangleStream<PS_INPUT> SpriteStream)
{
	[unroll]
	for (int i = 0; i < 4; i++) {
		PS_INPUT Out = (PS_INPUT)0;
		//float4 Pos = In[0].Pos * g_positions[i].x + width * float4(g_positions[i], 0, 0);
		float4 Pos =
			In[0].Pos * g_posFlag[i].x
			+ In[0].NextPos * g_posFlag[i].y
			+ In[0].Tangent * g_tanFlag[i].x * width
			+ In[0].NextTan * g_tanFlag[i].y * width;
		Pos.w = 1;
		Out.Pos = mul(Pos, g_mWVP);
		Out.Color = float4(1.0, 0.0, 0.0, 1.0);
		Out.Tex = g_texcoords[i];
		SpriteStream.Append(Out);
	}
	SpriteStream.RestartStrip();
}