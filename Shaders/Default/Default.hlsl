
// Constant buffer per object
cbuffer cbPerObject : register(b0)
{
    float4x4 g_worldViewProjMat;
}

struct VertexIn
{
    float3 PosL     : POSITION;
    float4 Color    : COLOR;
};

struct VertexOut
{
    // There is no Geometry Shader so the output vertex position must be in homegenous clip space with SV_Position semantic being set
    float4 PosH     : SV_Position;
    float4 Color    : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    // Transform to homegenous clip space
    vout.PosH = mul(float4(vin.PosL, 1.0f), g_worldViewProjMat);
    // Copy the color from vin to vout to be passed into the pixel shader
    vout.Color = vin.Color;
    return vout;
}

float4 PS(VertexOut vin) : SV_Target
{
    // Nothing to do, return the color as is
    return vin.Color;
}