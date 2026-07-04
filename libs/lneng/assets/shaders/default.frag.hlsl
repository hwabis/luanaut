struct DirectionalLight {
  float3 direction;
  float3 color;
};

Texture2D albedoTex : register(t0, space2);
SamplerState albedoSampler : register(s0, space2);

cbuffer LightUBO : register(b0, space3) {
  DirectionalLight lights[8];
  int lightCount;
}

cbuffer MaterialUBO : register(b1, space3) {
  float crescentMin;
}

struct VSOutput {
  float4 pos : SV_Position;
  float3 normal : TEXCOORD0;
  float2 uv : TEXCOORD1;
  float3 color : TEXCOORD2;
};

static const float3 AMBIENT = float3(0.1, 0.1, 0.1);

float4 fragMain(VSOutput input) : SV_Target {
  float4 albedo = albedoTex.Sample(albedoSampler, input.uv);
  float3 normal = normalize(input.normal);
  float3 result = AMBIENT;
  float3 hotspot = float3(0, 0, 0);

  for (int i = 0; i < lightCount; i++) {
    float diffuse = max(dot(normal, normalize(-lights[i].direction)), 0.0);
    result += lights[i].color * diffuse;

    float crescent = smoothstep(crescentMin, 1.0, diffuse);
    hotspot += lights[i].color * crescent;
  }

  float3 lit = result * albedo.rgb;
  return float4(lit + hotspot, albedo.a);
}
