//=============================================================================
//
// 2dシェーダ [vs_2d.hlsl]
// Author : yamashita Yasuaki
// LastUpdate : 2015/4/20
//
//=============================================================================

#include "inputVS.hlsli"
#include "outputVS.hlsli"

#include "world.hlsli"
#include "material.hlsli"

#include "proj.hlsli"
#include "uvAnim.hlsli"
#include "screenSize.hlsli"

//=============================================================================
// 頂点シェーダ
//=============================================================================
OutputVS main(InputVS vs) {
  OutputVS ovs = (OutputVS)0;
  ovs.posH = mul(float4(vs.pos,1.0f),gWorld);
  ovs.posH = mul(ovs.posH,gProj);
  ovs.posH /= ovs.posH.w;
  ovs.posH.x = (ovs.posH.x * gScreenSize.x - 0.5f) / gScreenSize.x;
  ovs.posH.y = (ovs.posH.y * gScreenSize.y + 0.5f) / gScreenSize.y;
  ovs.posH.w = 1.f;
  ovs.texCoord.x = vs.texCoord.x * gUVAnim.x + gUVAnim.z;
  ovs.texCoord.y = vs.texCoord.y * gUVAnim.y + gUVAnim.w;
  ovs.material = gMaterial;
  return ovs;
}

// EOF