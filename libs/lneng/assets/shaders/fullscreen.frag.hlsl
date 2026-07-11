cbuffer FadeUBO : register(b0, space3) {
  float4 fadeColor;
}

struct VSOutput {
  float4 pos : SV_Position;
  float2 uv : TEXCOORD0;
};

float4 fragMain(VSOutput input) : SV_Target {
  return fadeColor;
}
