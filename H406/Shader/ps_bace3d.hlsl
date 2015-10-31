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
  float4 col      : COLOR0;
  float4 norDepth : COLOR1;
  float4 pos      : COLOR2;
  float4 col2     : COLOR3;
};

// bace pixcel shader
OUTPUT_PS main(InputPS inPS)
{
  OUTPUT_PS output;
  output.col = inPS.material * tex2D(TexSamp0,inPS.texCoord);
  output.norDepth = float4((normalize(inPS.normal) + 1) * 0.5f,inPS.posH.z / gFar);
  output.pos = float4((inPS.posW / gFar + 1.f) * 0.5f,1);
  output.col2 = output.col;
  return output;
}
