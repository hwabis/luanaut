// View should have rotate only (no translate/scale)
cbuffer ViewProjUniform : register(b0, space1) {
  float4x4 viewProj;
};

struct VSInput {
  float3 pos : TEXCOORD0;
};

struct VSOutput {
  float4 pos : SV_Position;
  float3 viewDir : TEXCOORD0;
};

VSOutput vertMain(VSInput input) {
  VSOutput output;
  output.pos = mul(viewProj, float4(input.pos, 1.0));
  output.pos = output.pos.xyww;
  output.viewDir = input.pos;
  return output;
}
