
//グローバル
cbuffer global
{
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
};
//
//バーテックスシェーダー
//
float4 VS(float4 Pos : POSITION) : SV_POSITION
{
	Pos = mul(Pos,g_mWVP);
	return Pos;
}
//
//ピクセルシェーダー
//
float4 PS(float4 Pos : SV_POSITION) : SV_Target
{
	float4 color = float4(0,1,0,0);
	return color;
}