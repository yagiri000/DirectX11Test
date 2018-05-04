Texture2D<float4> Texture : register(t0);
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

    float2 uv = pin.TexCoord;

    uv.x = uv.x - fmod(uv.x, 10.0 * W);
    uv.y = uv.y - fmod(uv.y, 10.0 * H);

    float4 color = Texture.Sample(Sampler, uv);
    return color;
}