struct VIn
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

VOut main(VIn input)
{
    VOut output;
    output.position = input.position;
    output.color = input.color;
    
    return output;
}