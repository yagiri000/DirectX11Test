Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);

struct VSInputTx
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};

float4 main(VSInputTx pin) : SV_Target
{
    float4 color = Texture.Sample(Sampler, pin.TexCoord);
    return color;
}