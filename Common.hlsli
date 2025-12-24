struct VIn
{
    float3 position : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

cbuffer PerObject : register(b12)
{
    matrix World; // 64 bytes
    matrix WVP; // 64 bytes
}

cbuffer PerFrame : register(b11)
{
    float3 cameraPosition; // 12
    float padding; // 4
}