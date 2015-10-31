//==============================================================================
//
// ps_dirlight[ps_dirlight.hlsl]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "DLight.hlsli"

sampler normap : register(s0);

// bace pixcel shader
float4 main(float2 uv : TEXCOORD0) : COLOR{
  float3 normal = tex2D(normap,uv).xyz * 2 - 1;
  float4 color = 0;

  for(int i = 0; i < LIGHT_NUM; i++) {
    float intensity = -dot(normalize(gLightDir[i]),normalize(normal));
    intensity = intensity * 0.5f + 0.5f * gLightPower[i];
    color += intensity * gLightCol[i];
  }

  color = 1 - min(color,1);
  color.a = 1;

  return color;
}
