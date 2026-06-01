struct DirectionalLight {
  float3 direction;
  float3 color;
};

cbuffer LightUBO : register(b0, space3) {
  DirectionalLight lights[8];
  int lightCount;
}

struct VSOutput {
  float4 pos : SV_Position;
  float3 normal : TEXCOORD0;
  float2 uv : TEXCOORD1;
  float3 color : TEXCOORD2;
};

static const float3 AMBIENT = float3(0.1, 0.1, 0.1);

float4 fragMain(VSOutput input) : SV_Target {
  float3 normal = normalize(input.normal);
  float3 result = AMBIENT;

  for (int i = 0; i < lightCount; i++) {
    float diffuse = max(dot(normal, normalize(-lights[i].direction)), 0.0);
    result += lights[i].color * diffuse;
  }

  return float4(result, 1.0);
}
