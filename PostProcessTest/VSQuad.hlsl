struct VSInputTx
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};

VSInputTx main(uint vI : SV_VertexId)
{
    VSInputTx vout;

    float2 texcoord = float2(vI & 1, vI >> 1);
    vout.TexCoord = texcoord;

    vout.Position = float4((texcoord.x - 0.5f) * 2, -(texcoord.y - 0.5f) * 2, 0, 1);
    return vout;
}