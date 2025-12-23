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

cbuffer PerObjectCB
{
    matrix WVP;
    float4 ambientLightColor;
    float4 directionalLightColor;
    float4 directionalLightDirection;
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
    
    // final color, good idea to call saturate to keep values between 0 and 1.
    output.color = saturate(ambientLightColor + float4(directionalFinal, 1));
    
    // If you wish to have more accurate lighting, you will need to pass the normals from the vertex shader to the pixel shader
    // and calculate the lighting based on the interpolated normals inside the pixel shader instead. As an exercise, you can try
    // to achieve this.
    
    //output.color = float4(1, 1, 1, 1);
    
    return output;
}