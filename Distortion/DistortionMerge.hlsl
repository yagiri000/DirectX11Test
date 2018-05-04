Texture2D<float4> SceneTex : register(t0);
Texture2D<float4> DistortionTex : register(t1);
sampler Sampler : register(s0);

struct VSInputTx
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};


static const float W = 1 / 1280.0;
static const float H = 1 / 720.0;

float4 main(VSInputTx pin) : SV_Target
{
    float4 d4 = DistortionTex.Sample(Sampler, pin.TexCoord);
    float2 d = (d4.xy - 0.5) * 2.0;
    float2 uv = pin.TexCoord + float2(d.x * W, d.y * H) * 20.0;

    float4 color = SceneTex.Sample(Sampler, uv);
    return color;
}