
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

// uv空間の(0.5, 0.5)を中心とし，一定の角度領域のみを描画する
// 円形のゲージUI用

//グローバル
cbuffer global
{
	matrix g_mW;
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
	float4 g_mAngleStart_Dif; // xに開始角度，yに開始角度から何度描画するか入った変数
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
	float4 color = txDiffuse.Sample(samLinear, input.Tex);
	
	float2 dir = normalize(float2(input.Tex.x-0.5, input.Tex.y-0.5));

	float startAngle = g_mAngleStart_Dif.x;
	float2 start = float2(cos(startAngle), sin(startAngle));
	float endAngle = g_mAngleStart_Dif.x + g_mAngleStart_Dif.y;
	float2 end = float2(cos(endAngle), sin(endAngle));
	float2 ave = normalize(start + end);

	float angleDif = g_mAngleStart_Dif.y;
	dir = -sign(angleDif - PI) * dir;
	clip(dot(ave, dir) - cos(angleDif*0.5));
	
	return color;
}