//==============================================================================
//
// inputPS[inputPS.hlsli]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

#ifndef _INPUTPS_H_
#define _INPUTPS_H_

//==============================================================================
// bace
//==============================================================================
struct InputPS {
  float2 texCoord : TEXCOORD0;
  float3 normal   : TEXCOORD1;
  float3 posW     : TEXCOORD2;
  float4 posH     : TEXCOORD3;
  float4 material : COLOR0;
};

#endif
//EOF