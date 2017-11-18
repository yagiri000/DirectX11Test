//グローバル
cbuffer global
{
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
	float4 g_Alpha;
};

//構造体
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : TEXCOORD1;
};

static const float g_dither[16] = {
	0.0 / 16.0,
	8.0 / 16.0,
	2.0 / 16.0,
	10.0 / 16.0,

	12.0 / 16.0,
	4.0 / 16.0,
	14.0 / 16.0,
	6.0 / 16.0,
	
	3.0 / 16.0,
	11.0 / 16.0,
	1.0 / 16.0,
	9.0 / 16.0,
	
	15.0 / 16.0,
	7.0 / 16.0,
	13.0 / 16.0,
	5.0 / 16.0,
};

static const float width = 800;
static const float height = 600;

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 pos = mul(float4(input.Normal, 1.0), g_mWVP);
	pos.x = pos.x / pos.w;
	pos.y = pos.y / pos.w;
	uint ix = ((pos.x + 1.0) * 0.5) * width * 0.25;
	uint iy = ((pos.y + 1.0) * 0.5) * height * 0.25;
	//ix = ix % 4;
	//iy = iy % 4;
	ix = ix & 3;
	iy = iy & 3;
	if (g_Alpha.x < g_dither[ix + 4 * iy]) {
		discard;
	}
	float4 color = float4(1.0, 0.5, 0.0, 1.0);
	return color;
}