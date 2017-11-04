
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//ÉOÉçÅ[ÉoÉã
cbuffer global : register(b0)
{
	float4 diffuse;
};


struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_Target
{
	float4 color = txDiffuse.Sample(samLinear, input.Tex) * diffuse;
	return color;
}