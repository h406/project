//=============================================================================
//
// 頂点シェーダ [vs_instancing.hlsl]
// Author : yamashita Yasuaki
// LastUpdate : 2015/12/23
//
//=============================================================================

#include "outputVS.hlsli"

#include "world.hlsli"

// ビュープロジェクション行列
uniform float4x4 gVP;


struct InputVS {
  float3   pos      : POSITION0;
  float2   uv       : TEXCOORD0;
  float4   world1   : TEXCOORD1;
  float4   world2   : TEXCOORD2;
  float4   world3   : TEXCOORD3;
  float4   world4   : TEXCOORD4;
  float4   material : TEXCOORD5;
  float4   texCoord : TEXCOORD6;
};

//=============================================================================
// 頂点シェーダ
//=============================================================================
OutputVS main(InputVS vs) {
  OutputVS ovs = (OutputVS)0;
  float4x4 world = mul(float4x4(vs.world1,vs.world2,vs.world3,vs.world4),gWorld);
  float4x4 wvp   = mul(world,gVP);

  ovs.pos = mul(float4(vs.pos,1.0f),wvp);
  ovs.posH = mul(float4(vs.pos,1.0f),wvp);
  ovs.posW = mul(float4(vs.pos,1.0f),world).xyz;
  ovs.normal = mul(float3(0.f,1.f,0.f),(float3x3)world).xyz;
  ovs.texCoord.x = 0;
  ovs.texCoord.y = 0;
  ovs.material = vs.material;

  return ovs;
}

// EOF