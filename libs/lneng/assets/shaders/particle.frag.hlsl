struct VSOutput {
  float4 pos : SV_Position;
  float2 uv : TEXCOORD0;
  float alpha : TEXCOORD1;
};

float4 fragMain(VSOutput input) : SV_Target {
  float dist = length(input.uv);
  float alpha = smoothstep(0.5, 0.3, dist) * input.alpha;
  return float4(1, 1, 1, alpha);
}
