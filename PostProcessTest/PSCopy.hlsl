Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);

struct VSInputTx
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};


float4 main(VSInputTx pin) : SV_Target
{
    float WIDTH_RATE = 1.0 / 1280.0 * 0.5;
    float HEIGHT_RATE = 1.0 / 720.0 * 0.5;
    float DOWN_RATE = HEIGHT_RATE * 0.01;

    //float4 color = Texture.Sample(Sampler, pin.TexCoord) * 0.2;
    float4 colorG = 0.0;
    colorG += Texture.Sample(Sampler, pin.TexCoord + float2(WIDTH_RATE, 0.0 - DOWN_RATE)) * 0.25;
    colorG += Texture.Sample(Sampler, pin.TexCoord + float2(-WIDTH_RATE, 0.0 - DOWN_RATE)) * 0.25;
    colorG += Texture.Sample(Sampler, pin.TexCoord + float2(0.0, HEIGHT_RATE - DOWN_RATE)) * 0.25;
    colorG += Texture.Sample(Sampler, pin.TexCoord + float2(0.0, -HEIGHT_RATE - DOWN_RATE)) * 0.25;
    colorG.r = clamp(colorG.r, 0, 1);
    colorG.g = clamp(colorG.g, 0, 1);
    colorG.b = clamp(colorG.b, 0, 1);
    colorG.rgb *= 0.99;
    //colorG.gb *= 0.9;

    float4 color_ = Texture.Sample(Sampler, pin.TexCoord);
    
    float4 color = Texture.Sample(
    Sampler,
    pin.TexCoord 
    + float2(sin(color_.g * 10) * 0.001, cos(color_.b * 10) * 0.001)
    + float2(
    cos(cos(pin.TexCoord.y * 5) * 10) * WIDTH_RATE,
    cos(sin(pin.TexCoord.x * 5) * 10) * HEIGHT_RATE
    )
    );

    float4 color2 = Texture.Sample(
    Sampler,
    pin.TexCoord 
    + float2(sin(color_.b * 10) * 0.001, cos(color_.r * 10) * 0.001)
    + float2(
    cos(sin(pin.TexCoord.y * 10) * 10) * WIDTH_RATE,
    cos(cos(pin.TexCoord.x * 10) * 10) * HEIGHT_RATE
    )
    );

    float4 color3 = Texture.Sample(
    Sampler,
    pin.TexCoord 
    + float2(sin(color_.r * 10) * 0.001, cos(color_.g * 10) * 0.001)
    + float2(
    cos(cos(pin.TexCoord.y * 3) * 30) * WIDTH_RATE,
    sin(cos(pin.TexCoord.x * 3) * 30) * HEIGHT_RATE
    )
    );
    
    color.r = color.r - 0.2 / 255.0;
    color2.g = color2.g - 0.5 / 255.0;
    color3.b = color3.b - 0.5 / 255.0;
    color = float4(color.r, color2.g, color3.b, 1.0);

    return color * 0.9 + colorG * 0.1;
}