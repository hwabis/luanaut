struct VSInput {
  float3 pos;
  float3 normal;
  float2 uv;
};

struct VSOutput {
  float4 pos : SV_Position;
  float3 normal;
  float2 uv;
};

struct UniformBufferObject {
  float4x4 model;
  float4x4 view;
  float4x4 proj;
};

[[vk::binding(0, 0)]]
ConstantBuffer<UniformBufferObject> ubo;
[[vk::binding(1, 0)]]
Sampler2D texture;

[shader("vertex")]
VSOutput vertMain(VSInput input) {
  VSOutput output;
  output.pos = mul(ubo.proj, mul(ubo.view, mul(ubo.model, float4(input.pos, 1))));
  output.normal = mul((float3x3)ubo.model, input.normal);
  output.uv = input.uv;
  return output;
}

[shader("fragment")]
float4 fragMain(VSOutput vertIn) : SV_TARGET {
  const float3 lightDir = normalize(float3(1, 0.5, 0.2));
  float brightness = max(dot(vertIn.normal, lightDir), 0);
  float ambient = 0.05;
  float4 texColor = texture.Sample(vertIn.uv);
  return float4(texColor.rgb * (brightness + ambient), texColor.a);
}
