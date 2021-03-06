
Texture2D<float4> txCoords : register(t0);

//グローバル
cbuffer global : register(b0)
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
	float4 g_UVScroll;
};

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float4 Tangent : TANGENT;
	float4  Color : COLOR;
	float2 Tex : TEXCOORD;
	uint vertexId : SV_VertexID;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};


static const float PI = 3.141592;

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	float4 coord = txCoords.Load(uint3(input.vertexId, 0, 0));
	coord = coord * 2 - 1.0;
	float3 pos = coord;
	output.Pos = mul(float4(pos, 1), g_mWVP);
	output.Tex = input.Tex + g_UVScroll.xy;
	output.Color = input.Color;
	return output;
}