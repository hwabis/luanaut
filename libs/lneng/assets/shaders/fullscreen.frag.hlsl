cbuffer FadeUBO : register(b0, space3) {
  float4 fadeColor;
}

float4 fragMain(VSOutput input) : SV_Target {
  return fadeColor;
}
