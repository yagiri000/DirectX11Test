//グローバル
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
};

struct GS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};

static const float2 g_positions[4] = { float2(-1, 1), float2(1, 1), float2(-1, -1), float2(1, -1) };
static const float2 g_texcoords[4] = { float2(0, 1), float2(1, 1), float2(0, 0), float2(1, 0) };

[maxvertexcount(4)]
void main(point GS_INPUT In[1], inout TriangleStream<PS_INPUT> SpriteStream)
{
	[unroll]
	for (int i = 0; i < 4; i++) {
		PS_INPUT Out = (PS_INPUT)0;
		float4 Pos = In[0].Pos + 0.1 * float4(g_positions[i], 0, 0);
		Out.Pos = mul(Pos, g_mWVP);
		Out.Color = float4(1.0, 0.0, 0.0, 1.0);
		Out.Tex = g_texcoords[i];
		SpriteStream.Append(Out);
	}
	SpriteStream.RestartStrip();
}