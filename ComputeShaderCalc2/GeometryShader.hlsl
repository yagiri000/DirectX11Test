//グローバル
cbuffer global
{
	matrix g_mW;
	matrix g_mVP; //ワールド、ビュー、射影の合成変換行列
};

struct GS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD0;
};

static const float2 g_positions[4] = { float2(-1, 1), float2(1, 1), float2(-1, -1), float2(1, -1) };
static const float2 g_texcoords[4] = { float2(0, 1), float2(1, 1), float2(0, 0), float2(1, 0) };

[maxvertexcount(4)]
void main(point GS_INPUT In[1], inout TriangleStream<PS_INPUT> SpriteStream)
{
	[unroll]
	for (int i = 0; i < 4; i++) {
		PS_INPUT Out = (PS_INPUT)0;
		float4 Pos = 0.006 * float4(g_positions[i], 0, 0);
		Pos = mul(Pos, g_mW);
		Pos += In[0].Pos;
		Out.Pos = mul(Pos, g_mVP);
		Out.Color = In[0].Color;
		Out.Tex = g_texcoords[i];
		SpriteStream.Append(Out);
	}
	SpriteStream.RestartStrip();
}