Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);

struct VSInputTx
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};


float4 main(VSInputTx pin) : SV_Target
{
    float WIDTH_RATE = 1.0 / 1280.0;
    float HEIGHT_RATE = 1.0 / 720.0;

    float4 color_ = Texture.Sample(Sampler, pin.TexCoord);
    
    float4 color = Texture.Sample(
    Sampler,
    pin.TexCoord 
    + float2(sin(color_.g * 10) * 0.001, cos(color_.g * 10) * 0.001)
    + float2(
    cos(cos(pin.TexCoord.y * 5) * 10) * WIDTH_RATE,
    cos(sin(pin.TexCoord.x * 5) * 10) * HEIGHT_RATE
    )
    );

    float4 color2 = Texture.Sample(
    Sampler,
    pin.TexCoord 
    + float2(sin(color_.r * 10) * 0.001, cos(color_.r * 10) * 0.001)
    + float2(
    cos(cos(pin.TexCoord.y * 10) * 10) * WIDTH_RATE,
    sin(sin(pin.TexCoord.x * 10) * 10) * HEIGHT_RATE
    )
    );
    
    color.r = color.r - 0.2 / 255.0;
    color2.g = color2.g - 0.5 / 255.0;
    return float4(color.r, color2.g, 0.0, 1.0);
}