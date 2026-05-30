struct VSOutput {
  float4 pos : SV_Position;
  float3 normal : TEXCOORD0;
  float2 uv : TEXCOORD1;
  float3 color : TEXCOORD2;
};

// todo actual lighting with ubo
float4 fragMain(VSOutput input) : SV_Target {
  return float4(input.normal * 0.5 + 0.5, 1);
}
