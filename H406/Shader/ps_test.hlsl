//==============================================================================
//
// ps_test[ps_test.hlsl]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
sampler randmap : register(s0);
sampler posmap : register(s1);
sampler colmap : register(s2);

uniform float f = 0;

// bace pixcel shader
float4 main(float2 uv : TEXCOORD0) : COLOR{
  float2 centerOrigin = uv * 2.0 - 1.0;
  float cornerShade = 1.0 - pow(length(centerOrigin * 0.75),10.0);
  // sinを使ってノイズを表現
  float3 noiseLine = (3.0 + sin((uv.y + f + sin(uv.x*0.5)) * 200.0)) / 4.0;

    float2 test = abs(uv - 0.5f) * 2;
    for(int i = 0; i<3; i++) test *= test;
  float4 col = sin((test.x + test.y) / 2 * (3.14159265 * 0.5));
    col.a = 1;

  return float4(noiseLine * 0.5f,1) * tex2D(randmap,(tex2D(randmap,float2(f,f) % 1).xy + uv * (float2(1024,720) / float2(256,256) * 0.25f)) % 1) + col;
}
