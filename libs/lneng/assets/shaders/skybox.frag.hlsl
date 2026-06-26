TextureCube cubeMap : register(t0, space2);
SamplerState cubeSampler : register(s0, space2);

struct VSOutput {
  float4 pos : SV_Position;
  float3 viewDir : TEXCOORD0;
};

float4 fragMain(VSOutput input) : SV_Target {
    return cubeMap.Sample(cubeSampler, normalize(input.viewDir));
}
