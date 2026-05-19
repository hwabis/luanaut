struct VSOutput {
  float4 pos : SV_Position;
  float3 color : TEXCOORD0;
};

float4 fragMain(VSOutput input) : SV_Target {
  return float4(input.color, 1);
}
