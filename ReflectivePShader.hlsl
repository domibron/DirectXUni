Texture2D texture0 : register(t0);
TextureCube skybox0 : register(t1);
sampler sampler0;

struct PIn
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
    float3 uvw : TEXCOORD1;
};

float4 main(PIn input) : SV_TARGET
{
    float4 sampled = texture0.Sample(sampler0, input.uv);
    float4 reflectedSampled = skybox0.Sample(sampler0, input.uvw);
    clip(sampled.a < 0.1f ? -1 : 1);
    float4 combined = (input.color * sampled * 0.6) + (reflectedSampled * 0.4);
    return saturate(combined);
}