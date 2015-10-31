//==============================================================================
//
// ps_parpix[ps_parpix.hlsl]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "DLight.hlsli"

sampler normap : register(s0);
sampler posmap : register(s1);

uniform float  gFar;
uniform float3 gDLight[4];
uniform float3 gDLightCol[4];
uniform float3 gCameraPos;

// bace pixcel shader
float4 main(float2 uv : TEXCOORD0) : COLOR{
  float3 normal = tex2D(normap,uv).xyz * 2 - 1;
  float3 pos = (tex2D(posmap,uv).xyz * 2 - 1) * gFar;
  float3 color = 0;

  for(int i = 0; i < 4; i++) {
    // ���C�g�v�Z
    float3 dir = normalize(pos - gDLight[i]);
    float light = max(dot(normal,-dir),0);

    // �X�y�L�����v�Z
    float3 toEye = normalize(gCameraPos - pos);	// �J��������_�̃x�N�g��
    float3 r = reflect(dir,normalize(normal));	// ����
    float  s = pow(max(dot(r,toEye),0.0f),5.0f);		// �X�y�L�����p���[

    // ���̌���
    float d = distance(gDLight[i],pos);	// ����
    float a = 0.1f + 0.01f * d + 0.f * d * d;

    // ���C�e�B���O�v�Z
    float3 lighting = light * gDLightCol[i] + s * gDLightCol[i] * float3(1.0f,1.0f,1.0f);
    color += lighting.rgb / a;
  }

  return float4(color,1);
}
