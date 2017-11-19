//グローバル
cbuffer global : register(b0)
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
	float4 g_UVScroll;
	float4 g_Life;
};

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : TARGETPOS; // NORMALとして使用せず，Lerp先を設定する
	float2 Tex : TEXCOORD;
	float4 Color : COLOR;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 UV : TEXCOORD1;
	float2 Tex : TEXCOORD2;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	float3 pos = lerp(input.Pos, input.Normal, g_Life.x);
	output.Pos = mul(float4(pos, 1), g_mWVP);
	output.UV = input.Tex + float2(g_UVScroll.x, g_UVScroll.y);
	output.Tex = input.Tex ;
	output.Color = input.Color;
	return output;
}