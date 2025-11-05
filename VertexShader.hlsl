struct VIn
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

cbuffer PerObjectCB
{
    float3 pos;
    float padding;
};

VOut main(VIn input)
{
    VOut output;
    output.position = float4(input.position + pos, 1);
    output.color = input.color;
    
    return output;
}