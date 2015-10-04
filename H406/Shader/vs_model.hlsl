//=============================================================================
//
// 頂点シェーダ [vs_model.hlsl]
// Author : yamashita Yasuaki
// LastUpdate : 2015/4/20
//
//=============================================================================

#include "inputVS.hlsli"
#include "outputVS.hlsli"

#include "world.hlsli"
#include "wvp.hlsli"
#include "material.hlsli"

//=============================================================================
// 頂点シェーダ
//=============================================================================
OutputVS main(InputVS vs) {
  OutputVS ovs = (OutputVS)0;

  ovs.posH = mul(float4(vs.pos,1.0f),gWVP);
  ovs.posW = mul(float4(vs.pos,1.0f),gWorld).xyz;
  ovs.normal = mul(float4(vs.normal,0.0f),gWorld).xyz;
  ovs.texCoord = vs.texCoord;
  ovs.material = gMaterial;


  return ovs;
}

// EOF