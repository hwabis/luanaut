struct VSInput {
  float3 pos : TEXCOORD0;
  float3 color : TEXCOORD1;
};

struct VSOutput {
  float4 pos : SV_Position;
  float3 color : TEXCOORD0;
};

VSOutput vertMain(VSInput input) {
  VSOutput output;
  output.pos = float4(input.pos, 1);
  output.color = input.color;
  return output;
}
