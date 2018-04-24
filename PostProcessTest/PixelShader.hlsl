
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//グローバル
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
	float4 g_mLife;
};


struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : TEXCOORD0;
	float2 UV : TEXCOORD1;
	float2 Tex : TEXCOORD2;
};

static const float PI = 3.14159265;
static const float PI2 = 3.14159265 * 2;

float4 main(PS_INPUT input) : SV_Target
{
	//float4 color = txDiffuse.Sample(samLinear, input.UV);
	float4 color = 1;

	float x = input.Tex.x;
	float y = input.Tex.y;
	float t = g_mLife.x;
	x = x * 2 - 1;
	y = y * 2 - 1;
	float r = sqrt(x * x + y * y);
	float angleN = (atan2(y, x) + PI) / PI2; // 0~1に正規化された角度

	//float r1 = g_mLife;
	//float r0 = r1 - 0.1;
	//if (!(r < r1 && r > r0)) {
	//	discard;
	//}


	if (sin(r * 16.0 + t * 30.0 + angleN * 32.0) < 0.0f) {
		discard;
	}
	return color;
}