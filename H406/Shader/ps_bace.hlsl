//==============================================================================
//
// ps_bace[ps_bace.hlsl]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "inputPS.hlsli"
#include "texSamp0.hlsli"

// bace pixcel shader
float4 main(InputPS inPS) : COLOR0
{
  return inPS.material * tex2D(TexSamp0,inPS.texCoord);
}

//EOF
