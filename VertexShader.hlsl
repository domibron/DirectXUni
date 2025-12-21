struct VIn
{
    float3 position : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VOut
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

cbuffer PerObjectCB
{
    matrix WVP;
};

VOut main(VIn input)
{
    VOut output;
    output.position = mul(WVP, float4(input.position, 1));
    //output.color = input.color;
    output.color = float4(1, 1, 1, 1);
    output.uv = input.uv;
    
    return output;
}