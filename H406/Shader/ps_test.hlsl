//==============================================================================
//
// ps_test[ps_test.hlsl]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
sampler norDepth : register(s0);
sampler pos : register(s1);
sampler color : register(s2);


// bace pixcel shader
float4 main(float2 uv : TEXCOORD0) : COLOR{
  float fog = min(max(0,tex2D(norDepth,uv).a),1);
  fog = (fog*2) * (fog*2) / 4;
  float4 col = fog * float4(1,1,1,1);
  //col += (1.f - fog) * tex2D(color,uv);
  col.a = 1;
  return col;
}
