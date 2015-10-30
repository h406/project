//==============================================================================
//
// ps_bace3d[ps_bace3d.hlsl]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************

struct PS_IN {
  float2 uv1 : TEXCOORD0;
  float2 uv2 : TEXCOORD1;
  float2 uv3 : TEXCOORD2;
  float2 uv4 : TEXCOORD3;
};

sampler TexSamp0;

// bace pixcel shader
float4 main(PS_IN psin) : COLOR{

  // 判定の値
  float4 n1 = tex2D(TexSamp0,psin.uv1);// 左上
  float4 n2 = tex2D(TexSamp0,psin.uv2);// 右下
  float4 n3 = tex2D(TexSamp0,psin.uv3);// 左下
  float4 n4 = tex2D(TexSamp0,psin.uv4);// 右上

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

  //float3 col1 = (tex2D(TexSamp0,psin.uv1).rgb - tex2D(TexSamp0,psin.uv2).rgb) * 2;
  //float3 col2 = (tex2D(TexSamp0,psin.uv3).rgb - tex2D(TexSamp0,psin.uv4).rgb) * 2;
  //
  //float s1= dot(col1,col1).r * 4;
  //float s2= dot(col2,col2).r * 4;
  //
  //
  ////周辺のピクセル位置のＺ値を取得
  //float Z1 = tex2D(TexSamp0,psin.uv1).a;
  //float Z2 = tex2D(TexSamp0,psin.uv2).a;
  //float Z3 = tex2D(TexSamp0,psin.uv3).a;
  //float Z4 = tex2D(TexSamp0,psin.uv4).a;
  //
  ////周辺のピクセル位置のＺ値と比較し、m_Limitより大きいところがあったらエッジ部分と判定する
  //if(abs(Z1 - Z2) < 0.02f &&
  //  abs(Z1 - Z3) < 0.02f &&
  //  abs(Z1 - Z4) < 0.02f) {
  //  return float4(0,0,0,1 - ((1 - s1) + s2));
  //  //return float4((1 - col1) + col2,1);
  //}
  //
  //return float4(0,0,0,1);


  //return 

  // RGB 各輝度差を取得
  //sub_x2 r0,t0,t1    // r0 = 2*(t0-t1)
  //sub_x2 r1,t2,t3    // r1 = 2*(t2-t3)
  //
  //// それぞれの輝度差を二乗して絶対値の近似を取得
  //dp3_x4 r0,r0,r0    // r0 = 16*(t0-t1)^2
  //dp3_x4 r1,r1,r1    // r1 = 16*(t2-t3)^2
  //
  //// 1 - 全ての輝度差 で白地に黒エッジ
  //add r0,1 - r0,-r1     // r0 = 1-16*((t0-t1)^2+(t2-t3)^2)

  //return float4(0,1,0,0.5);
}
