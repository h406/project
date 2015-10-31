//==============================================================================
//
// ps_bace3d[ps_bace3d.hlsl]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************

sampler norDepth : register(s0);

// bace pixcel shader
float4 main(float2 uv : TEXCOORD0) : COLOR{

  float2 pixel = float2(1.0f / 1280.f,1.0f / 720.f);

  float z = (1 - sin(tex2D(norDepth,uv).a * (3.14159265f * 0.5f))) * 2.5f;
  z = max(0.5f,z);

  // 判定の値
  float4 n1 = tex2D(norDepth,uv + float2(-z,-z) * pixel); // 左上
  float4 n2 = tex2D(norDepth,uv + float2(z,z) * pixel); // 右下
  float4 n3 = tex2D(norDepth,uv + float2(-z,z) * pixel); // 左下
  float4 n4 = tex2D(norDepth,uv + float2(z,-z) * pixel); // 右上

  if(abs(n1.a - n2.a) < 0.02f &&
    abs(n1.a - n3.a) < 0.02f &&
    abs(n1.a - n4.a) < 0.02f) {

    // 対角から差分を計算
    float4 diagonalDelta = abs(n1 - n2) + abs(n3 - n4);

    // Zバッファの差分で線を書く
    float depthDelta = saturate((diagonalDelta.a - 0.5f) * 10.f);

    // 法線の差分を書く
    float normalDelta = dot(diagonalDelta.rgb,1);
    normalDelta = saturate((normalDelta - 0.5f) * 10.f);

    // 差を計算
    float edgeValue = saturate(depthDelta + normalDelta) * 2.0f;

    return float4(0,0,0,(edgeValue));
  }

  return float4(0,0,0,1);
}
