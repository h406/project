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
#include "far.hlsli"

struct OUTPUT_PS {
  float4 col : COLOR0;
  float4 nor : COLOR1;
  float4 depth: COLOR2;
  float4 col2 : COLOR3;
};

// bace pixcel shader
OUTPUT_PS main(InputPS inPS)
{
  OUTPUT_PS output;

  output.nor = float4((normalize(inPS.normal) + 1) * 0.5f, 1);
  output.depth = inPS.posH.z / gFar;
  output.depth.a = 1;
  
  output.col = inPS.material * tex2D(TexSamp0,inPS.texCoord);
  output.col2 = output.col;
  return output;
}

