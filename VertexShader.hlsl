#define MAX_POINT_LIGHTS 8

struct VIn
{
    float3 position : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
    float3 norm : NORMAL;
};

struct VOut
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

struct PointLight
{
    float4 position; // 16
    
    float4 color; // 16
    
    float strength; // 4
    bool enabled; // 4
    float2 padding; // 8
};

cbuffer PerObjectCB
{
    matrix WVP; // 64
    float4 ambientLightColor; // 16
    float4 directionalLightColor; // 16
    float4 directionalLightDirection; // 16
    PointLight pointLights[MAX_POINT_LIGHTS]; // 384 bytes
};

VOut main(VIn input)
{
    VOut output;
    output.position = mul(WVP, float4(input.position, 1));
    //output.color = input.color;
    output.uv = input.uv;
    
    // lighting
    float diffuseAmount = dot(directionalLightDirection.xyz, input.norm);
    
    // clamp the light direction (works the same as clamping)
    diffuseAmount = saturate(diffuseAmount);
    
    float3 directionalFinal = directionalLightColor * diffuseAmount;
    
    // point lighting
    float3 pointFinal = float3(0, 0, 0);
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        if (!pointLights[i].enabled)
            continue;
                
        float3 pointLightDir = normalize(pointLights[i].position.xyz - input.position);
        float pointLightDistance = length(pointLights[i].position.xyz - input.position);
        float pointLightAttenuation = pointLights[i].strength / (pointLightDistance * pointLightDistance + pointLights[i].strength);
        float pointAmount = dot(pointLightDir, input.norm) * pointLightAttenuation;
        pointAmount = saturate(pointAmount);
        pointFinal += pointLights[i].color * pointAmount;
    }
    
    
    // final color, good idea to call saturate to keep values between 0 and 1.
    output.color = saturate(ambientLightColor + float4(directionalFinal, 1) + float4(pointFinal, 1));
    
    // If you wish to have more accurate lighting, you will need to pass the normals from the vertex shader to the pixel shader
    // and calculate the lighting based on the interpolated normals inside the pixel shader instead. As an exercise, you can try
    // to achieve this.
    
    //output.color = float4(1, 1, 1, 1);
    
    return output;
}