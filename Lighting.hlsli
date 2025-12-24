#define MAX_POINT_LIGHTS 8

struct DirectionalLight
{
    float4 transposedDirection; // 16
    float4 colour; // 16
};

struct PointLight
{
    float4 position; // 16
    float4 color; // 16
    float strength; // 4
    bool enabled; // 4
    float2 padding; // 8
};

cbuffer LightingData : register(b13)
{
    float4 ambientLightCol; // 16
    DirectionalLight dirLight; // 32
    PointLight pointLights[MAX_POINT_LIGHTS]; // 384 bytes
}

float3 CalculateDirectionContribution(DirectionalLight light, float3 vNormal)
{
    float diffuseAmount = dot(normalize(light.transposedDirection.xyz), vNormal);
    diffuseAmount = saturate(diffuseAmount);
    return light.colour * diffuseAmount;
}

float3 CalculatePointLights(PointLight pointLights[MAX_POINT_LIGHTS], float4 vPosition, float3 vNormal)
{
    float3 pointFinal = float3(0, 0, 0);
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        if (!pointLights[i].enabled)
            continue;
        
        float4 pointLightDir = normalize(pointLights[i].position - vPosition);
        float pointLightDistance = length(pointLights[i].position - vPosition);
        float pointLightAttenuation = pointLights[i].strength / (pointLightDistance * pointLightDistance + pointLights[i].strength);
        float pointAmount = dot(pointLightDir.xyz, vNormal) * pointLightAttenuation;
        pointAmount = saturate(pointAmount);
        pointFinal += pointLights[i].color * pointAmount;
    }
    
     // If you wish to have more accurate lighting, you will need to pass the normals from the vertex shader to the pixel shader
    // and calculate the lighting based on the interpolated normals inside the pixel shader instead. As an exercise, you can try
    // to achieve this.
    
    return pointFinal;
}

float3 CalculateAllLighting(float3 amientColor, DirectionalLight light, PointLight pointLights[MAX_POINT_LIGHTS], float4 vPosition, float3 vNormal)
{
    float3 directional = CalculateDirectionContribution(light, vNormal);
    float3 pointTotal = CalculatePointLights(pointLights, vPosition, vNormal);
    return saturate(amientColor + directional + pointTotal);
}

float3 CalculateReflectionUVW(matrix World, float4 vertexPos, float3 vertexNorm, float3 cameraPos)
{
    // Vertex position in world space
    float3 wpos = mul(World, vertexNorm);
    
    // Surface normal in world space
    float3 wnormal = normalize(mul(World, float4(vertexNorm, 0)));
    
    // Obtain eye vector
    float3 eyeDir = normalize(cameraPos - wpos);
    
    // Compute reflection vector
    // return 2.0 * dot(eyeDir, wnormal) * wnormal - eyeDir;
    return reflect(-eyeDir, wnormal);

}