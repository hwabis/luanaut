struct VSInput {
  float3 pos;
  float3 normal;
};

struct VSOutput {
  float4 pos : SV_Position;
  float3 normal;
};

struct UniformBufferObject {
  float4x4 model;
  float4x4 view;
  float4x4 proj;
};

[[vk::binding(0, 0)]]
ConstantBuffer<UniformBufferObject> ubo;

[shader("vertex")]
VSOutput vertMain(VSInput input) {
  VSOutput output;
  output.pos = mul(ubo.proj, mul(ubo.view, mul(ubo.model, float4(input.pos, 1))));
  output.normal = mul((float3x3)ubo.model, input.normal);
  return output;
}

[shader("fragment")]
float4 fragMain(VSOutput vertIn) : SV_TARGET {
  const float3 lightDir = normalize(float3(1, 0.5, 0.2));
  const float4 objectColor = float4(1, 1, 1, 1);
  float brightness = max(dot(vertIn.normal, lightDir), 0);
  float ambient = 0.05;
  return float4(objectColor.rgb * (brightness + ambient), 1);
}
