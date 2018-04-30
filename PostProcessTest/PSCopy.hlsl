Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);

struct VSInputTx
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};


float4 main(VSInputTx pin) : SV_Target
{
    float WIDTH_RATE = 0.5 / 1280.0;
    float HEIGHT_RATE = 0.5 / 720.0;

    float4 base = Texture.Sample(Sampler, pin.TexCoord);
    
    float4 color = Texture.Sample(Sampler, pin.TexCoord) * 0.2;
    float dr = sin(base.b * 10) * -2.0 + 2.0;
    color += Texture.Sample(Sampler, pin.TexCoord + float2(WIDTH_RATE, HEIGHT_RATE)) * 0.3;
    color += Texture.Sample(Sampler, pin.TexCoord + float2(-WIDTH_RATE, HEIGHT_RATE)) * 0.3;
    color += Texture.Sample(Sampler, pin.TexCoord + float2(-WIDTH_RATE, -HEIGHT_RATE)) * 0.3;
    color += Texture.Sample(Sampler, pin.TexCoord + float2(WIDTH_RATE, -HEIGHT_RATE)) * 0.3;
    color -= Texture.Sample(Sampler, pin.TexCoord + float2(WIDTH_RATE * dr, HEIGHT_RATE * dr)) * 0.1;
    color -= Texture.Sample(Sampler, pin.TexCoord + float2(-WIDTH_RATE * dr, HEIGHT_RATE * dr)) * 0.1;
    color -= Texture.Sample(Sampler, pin.TexCoord + float2(-WIDTH_RATE * dr, -HEIGHT_RATE * dr)) * 0.1;
    color -= Texture.Sample(Sampler, pin.TexCoord + float2(WIDTH_RATE * dr, -HEIGHT_RATE * dr)) * 0.1;

    float4 colorG = Texture.Sample(Sampler, pin.TexCoord + float2(0.005, 0.0)) * 0.2;
    float dg = sin(base.r * 10) * -1.0 + 2.0;
    colorG += Texture.Sample(Sampler, pin.TexCoord + float2(WIDTH_RATE, 0.0)) * 0.3;
    colorG += Texture.Sample(Sampler, pin.TexCoord + float2(-WIDTH_RATE, 0.0)) * 0.3;
    colorG += Texture.Sample(Sampler, pin.TexCoord + float2(0.0, HEIGHT_RATE)) * 0.3;
    colorG += Texture.Sample(Sampler, pin.TexCoord + float2(0.0, -HEIGHT_RATE)) * 0.3;
    colorG -= Texture.Sample(Sampler, pin.TexCoord + float2(WIDTH_RATE * dg, 0.0)) * 0.1;
    colorG -= Texture.Sample(Sampler, pin.TexCoord + float2(-WIDTH_RATE * dg, 0.0)) * 0.1;
    colorG -= Texture.Sample(Sampler, pin.TexCoord + float2(0.0, HEIGHT_RATE * dg)) * 0.1;
    colorG -= Texture.Sample(Sampler, pin.TexCoord + float2(0.0, -HEIGHT_RATE * dg)) * 0.1;

    float4 colorB = Texture.Sample(Sampler, pin.TexCoord) * 0.2;
    float d = sin(base.g * 10) * 1.0 + 2.0;
    colorB += Texture.Sample(Sampler, pin.TexCoord + float2(WIDTH_RATE, 0.0)) * 0.3;
    colorB += Texture.Sample(Sampler, pin.TexCoord + float2(-WIDTH_RATE, 0.0)) * 0.3;
    colorB += Texture.Sample(Sampler, pin.TexCoord + float2(0.0, HEIGHT_RATE)) * 0.3;
    colorB += Texture.Sample(Sampler, pin.TexCoord + float2(0.0, -HEIGHT_RATE)) * 0.3;
    colorB -= Texture.Sample(Sampler, pin.TexCoord + float2(WIDTH_RATE * d, 0.0)) * 0.1;
    colorB -= Texture.Sample(Sampler, pin.TexCoord + float2(-WIDTH_RATE * d, 0.0)) * 0.1;
    colorB -= Texture.Sample(Sampler, pin.TexCoord + float2(0.0, HEIGHT_RATE * d)) * 0.1;
    colorB -= Texture.Sample(Sampler, pin.TexCoord + float2(0.0, -HEIGHT_RATE * d)) * 0.1;
    
    
    color.r = clamp(color.r, 0, 1);
    colorG.g = clamp(colorG.g, 0, 1);
    colorB.b = clamp(colorB.b, 0, 1);
    color.r *= 0.999;
    colorG.g *= 0.999;
    colorB.b *= 0.999;
    //colorG.rgb *= 0.96;
    //colorB.rgb *= 0.95;



    return float4(color.r, colorG.g, colorB.b, color.a);
}