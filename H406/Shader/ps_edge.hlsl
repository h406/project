//==============================================================================
//
// ps_bace3d[ps_bace3d.hlsl]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************

sampler TexSamp0 : register(s0);
sampler TexSamp1 : register(s1);

// bace pixcel shader
float4 main(float2 uv : TEXCOORD0) : COLOR{

  float2 pixel = float2(1.0f / 1280.f,1.0f / 720.f);

  float z = (1 - sin(tex2D(TexSamp1,uv).r * (3.14159265f * 0.5f))) * 2.5f;
  z = max(0.5f,z);

  // ����̒l
  float4 n1 = tex2D(TexSamp0,uv + float2(-z,-z) * pixel); // ����
  float4 n2 = tex2D(TexSamp0,uv + float2( z, z) * pixel); // �E��
  float4 n3 = tex2D(TexSamp0,uv + float2(-z, z) * pixel); // ����
  float4 n4 = tex2D(TexSamp0,uv + float2( z,-z) * pixel); // �E��

  n1.a = tex2D(TexSamp1,uv + float2(-z,-z) * pixel).r;
  n2.a = tex2D(TexSamp1,uv + float2(z,z) * pixel).r;
  n3.a = tex2D(TexSamp1,uv + float2(-z,z) * pixel).r;
  n4.a = tex2D(TexSamp1,uv + float2(z,-z) * pixel).r;

  if(abs(n1.a - n2.a) < 0.02f &&
    abs(n1.a - n3.a) < 0.02f &&
    abs(n1.a - n4.a) < 0.02f) {

    // �Ίp���獷�����v�Z
    float4 diagonalDelta = abs(n1 - n2) + abs(n3 - n4);

    // Z�o�b�t�@�̍����Ő�������
    float depthDelta = saturate((diagonalDelta.a - 0.5f) * 10.f);

    // �@���̍���������
    float normalDelta = dot(diagonalDelta.rgb,1);
    normalDelta = saturate((normalDelta - 0.5f) * 10.f);

    // �����v�Z
    float edgeValue = saturate(depthDelta + normalDelta) * 2.0f;

    return float4(0,0,0,(edgeValue));
  }

  return float4(0,0,0,1);
}
