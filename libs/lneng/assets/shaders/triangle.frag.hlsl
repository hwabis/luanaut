struct VSOutput {
  float4 pos : SV_Position;
  float3 normal : TEXCOORD0;
  float2 uv : TEXCOORD1;
  float3 color : TEXCOORD2;
};

float4 fragMain(VSOutput input) : SV_Target {
  return float4(input.color, 1);
}
