#include "Common.hlsli"
#include "Lighting.hlsli"

struct VOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
    float3 uvw : TEXCOORD1;
};

cbuffer LightingData// : register(b13)
{
    float4 ambientLightCol; // 16
    DirectionalLight dirLight; // 32
    PointLight pointLights[MAX_POINT_LIGHTS]; // 384 bytes
}

VOut main(VIn input)
{
    VOut output;
    
    // position
    output.position = mul(WVP, float4(input.position, 1));
    
    //output.color = input.color;
    
    // Texture coords
    output.uv = input.uv;
    
    // lighting (Color)
    output.color = float4(CalculateAllLighting(ambientLightCol.xyz, dirLight, pointLights, float4(input.position, 1), input.normal), 1);
    
    output.uvw = CalculateReflectionUVW(World, float4(input.position, 1), input.normal, cameraPosition);
    
    return output;
}