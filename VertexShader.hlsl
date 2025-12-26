#include "Common.hlsli"
#include "Lighting.hlsli"

struct VOut
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

VOut main(VIn input)
{
    VOut output;
    
    // position
    output.position = mul(WVP, float4(input.position, 1));
    
    //output.color = input.color;
    
    // Texture coords
    output.uv = input.uv;
    
    // lighting (Color)
    //output.color = float4(CalculateAllLighting(ambientLightCol.xyz, dirLight, pointLights, float4(input.position, 1), input.normal), 1);
    output.color = float4(1,1,1, 1);
    
    return output;
}