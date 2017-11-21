//グローバル
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
};


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

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD1;
};

static const float2 g_positions[4] = { float2(-1, 1), float2(1, 1), float2(-1, -1), float2(1, -1) };
static const float2 g_texcoords[4] = { float2(0, 1), float2(1, 1), float2(0, 0), float2(1, 0) };

[maxvertexcount(4)]
void main(point GS_INPUT In[1], inout TriangleStream<PS_INPUT> SpriteStream)
{
	if (In[0].Life_LifeVel.x < 1.0) {
		[unroll]
		for (int i = 0; i < 4; i++) {
			PS_INPUT Out = (PS_INPUT)0;
			float3 localPos =
				In[0].Right * In[0].Scale.x * g_positions[i].x
				+ In[0].Up * In[0].Scale.y * g_positions[i].y;

			float4 pos = In[0].Pos + float4(localPos, 1);
			pos.w = 1;
			Out.Pos = mul(pos, g_mWVP);
			Out.Color = In[0].Color;
			Out.Tex = g_texcoords[i];
			SpriteStream.Append(Out);
		}
		SpriteStream.RestartStrip();
	}
}