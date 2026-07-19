cbuffer ParticleUBO : register(b0, space1) {
  float4x4 viewProj;
  float3 cameraRight;
  float3 cameraUp;
  float3 particleWorldPos;
  float3 particleColor; // todo use this lol
  float particleSize;
  float alpha;
}

struct VSOutput {
  float4 pos : SV_Position;
  float2 uv : TEXCOORD0;
  float alpha : TEXCOORD1;
};

VSOutput vertMain(uint vertexId : SV_VertexID) {
  float2 corners[6] = {
    float2(-0.5,-0.5), float2(0.5,-0.5), float2(-0.5,0.5),
    float2(0.5,-0.5),  float2(0.5,0.5),  float2(-0.5,0.5)
  };
  float2 corner = corners[vertexId];

  float3 worldPos = particleWorldPos
                  + cameraRight * corner.x * particleSize
                  + cameraUp * corner.y * particleSize;

  VSOutput output;
  output.pos = mul(viewProj, float4(worldPos, 1.0));
  output.uv = corner;  // -0.5 to 0.5
  output.alpha = alpha;
  return output;
}
