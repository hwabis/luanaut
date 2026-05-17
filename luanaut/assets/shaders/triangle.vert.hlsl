struct VSOutput {
  float4 pos : SV_Position;
  float3 color : TEXCOORD0;
};

VSOutput vertMain(uint vertexID: SV_VertexID) {
  float2 positions[3] = {
    float2(0.0, 0.5),
    float2(-0.5, -0.5),
    float2(0.5, -0.5),
  };
  float3 colors[3] = {
    float3(1, 0, 0),
    float3(0, 1, 0),
    float3(0, 0, 1),
  };
  VSOutput output;
  output.pos = float4(positions[vertexID], 0, 1);
  output.color = colors[vertexID];
  return output;
}
