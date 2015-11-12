//==============================================================================
//
// ps_fxaa[ps_fxaa.hlsl]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
sampler textureSampler : register(s0);

// bace pixcel shader
float4 main(float2 uv : TEXCOORD0) : COLOR{
  // The parameters are hardcoded for now, but could be
  // made into uniforms to control fromt he program.
  float FXAA_SPAN_MAX = 8.0;
  float FXAA_REDUCE_MUL = 1.0 / 8.0;
  float FXAA_REDUCE_MIN = (1.0 / 128.0);
  float2 texcoordOffset = float2(1.f / 1280,1.f / 720);

  float3 rgbNW = tex2D(textureSampler,uv.xy + (float2(-1.0,-1.0) * texcoordOffset)).xyz;
  float3 rgbNE = tex2D(textureSampler,uv.xy + (float2(+1.0,-1.0) * texcoordOffset)).xyz;
  float3 rgbSW = tex2D(textureSampler,uv.xy + (float2(-1.0,+1.0) * texcoordOffset)).xyz;
  float3 rgbSE = tex2D(textureSampler,uv.xy + (float2(+1.0,+1.0) * texcoordOffset)).xyz;
  float3 rgbM = tex2D(textureSampler,uv.xy).xyz;

  float3 luma = float3(0.299,0.587,0.114);
  float lumaNW = dot(rgbNW,luma);
  float lumaNE = dot(rgbNE,luma);
  float lumaSW = dot(rgbSW,luma);
  float lumaSE = dot(rgbSE,luma);
  float lumaM = dot(rgbM,luma);

  float lumaMin = min(lumaM,min(min(lumaNW,lumaNE),min(lumaSW,lumaSE)));
  float lumaMax = max(lumaM,max(max(lumaNW,lumaNE),max(lumaSW,lumaSE)));

  float2 dir;
  dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
  dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));

  float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),FXAA_REDUCE_MIN);

  float rcpDirMin = 1.0 / (min(abs(dir.x),abs(dir.y)) + dirReduce);

  dir = min(float2(FXAA_SPAN_MAX,FXAA_SPAN_MAX),
    max(float2(-FXAA_SPAN_MAX,-FXAA_SPAN_MAX),dir * rcpDirMin)) * texcoordOffset;

  float3 rgbA = (1.0 / 2.0) * (
    tex2D(textureSampler,uv.xy + dir * (1.0 / 3.0 - 0.5)).xyz +
    tex2D(textureSampler,uv.xy + dir * (2.0 / 3.0 - 0.5)).xyz);
  float3 rgbB = rgbA * (1.0 / 2.0) + (1.0 / 4.0) * (
    tex2D(textureSampler,uv.xy + dir * (0.0 / 3.0 - 0.5)).xyz +
    tex2D(textureSampler,uv.xy + dir * (3.0 / 3.0 - 0.5)).xyz);
  float lumaB = dot(rgbB,luma);

  float4 color;
  if((lumaB < lumaMin) || (lumaB > lumaMax)) {
    color.xyz = rgbA;
  }
  else {
    color.xyz = rgbB;
  }
  color.a = 1.0;

  return color;// *vertColor;
}
