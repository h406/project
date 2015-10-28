//==============================================================================
//
// ps_bace3d[ps_bace3d.hlsl]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "inputPS.hlsli"
#include "texSamp0.hlsli"
#include "DLight.hlsli"

// bace pixcel shader
float4 main(InputPS inPS) : COLOR0
{
  // ”½ŽË‚·‚éŒõ‚Ì‹­‚³
  float intensity = -dot(normalize(gDLight.direction),normalize(inPS.normal));
  float4 color = 1;

  color = intensity * gDLight.color;
  color.a = gDLight.color.a;

  return color * tex2D(TexSamp0,inPS.texCoord);
}