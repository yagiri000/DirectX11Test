//グローバル
cbuffer global
{
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
};

float4 main() : SV_TARGET
{
	return float4(1.0f, 0.5f, 0.0f, 1.0f);
}