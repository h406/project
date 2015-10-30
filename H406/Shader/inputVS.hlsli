//==============================================================================
//
// inputVS[inputVS.hlsli]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

#ifndef _INPUTVS_H_
#define _INPUTVS_H_

//==============================================================================
// bace
//==============================================================================
struct InputVS {
  float3 pos      : POSITION0;
  float3 normal   : NORMAL;
  float2 texCoord : TEXCOORD0;
};

#endif
//EOF