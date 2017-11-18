//グローバル
cbuffer global
{
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
	float4 alpha;
};

//構造体
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : TEXCOORD1; // NORMALでもTEXCOORDでもない
};


VS_OUTPUT main(float4 pos : POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	pos.w = 1;
	output.Pos = mul(pos, g_mWVP);
	output.Normal = pos;
	return output;
}