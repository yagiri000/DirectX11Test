//グローバル
cbuffer global
{
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
};

float4 main(float4 pos : POSITION) : SV_POSITION
{
	pos = mul(pos,g_mWVP);
	return pos;
}