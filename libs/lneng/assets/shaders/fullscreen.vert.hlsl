struct VSOutput {
  float4 pos : SV_Position;
  float2 uv : TEXCOORD0;
};

VSOutput vertMain(uint vertexId : SV_VertexID) {
  VSOutput output;

  float2 positions[3] = {
    float2(-1.0, -1.0),
    float2( 3.0, -1.0),
    float2(-1.0,  3.0),
  };

  float2 uvs[3] = {
    float2(0.0, 0.0),
    float2(2.0, 0.0),
    float2(0.0, 2.0),
  };

  float2 pos = positions[vertexId];
  output.pos = float4(pos, 0.0, 1.0);
  output.uv = uvs[vertexId];

  return output;
}
