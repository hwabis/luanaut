cbuffer MvpUniform : register(b0, space1) {
  float4x4 mvp;
};
cbuffer NormalUniform : register(b1, space1) {
  float4x4 normalMatrix; // Really a 3x3 (4x4 only for alignment)
};

struct VSInput {
  float3 pos : TEXCOORD0;
  float3 normal : TEXCOORD1;
  float2 uv : TEXCOORD2;
  float3 color : TEXCOORD3;
};

struct VSOutput {
  float4 pos : SV_Position;
  float3 normal : TEXCOORD0;
  float2 uv : TEXCOORD1;
  float3 color : TEXCOORD2;
};

VSOutput vertMain(VSInput input) {
  VSOutput output;
  output.pos = mul(mvp, float4(input.pos, 1.0));
  output.normal = mul((float3x3)normalMatrix, input.normal);
  output.uv = input.uv;
  output.color = input.color;
  return output;
}
