
//==============================================================================
//
// outputVS[outputVS.hlsli]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

#ifndef _OUTPUTVS_H_
#define _OUTPUTVS_H_

//==============================================================================
// bace
//==============================================================================
struct OutputVS {
  float4 pos      : POSITION0;
  float2 texCoord : TEXCOORD0;
  float3 normal   : TEXCOORD1;
  float3 posW     : TEXCOORD2;
  float4 posH     : TEXCOORD3;
  float4 material : COLOR0;
};

#endif
//EOF
