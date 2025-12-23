struct VIn
{
    float3 position : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

cbuffer PerObject : register(b12)
{
    matrix WVP; // 64 bytes
}