//=============================================================================
//
// ïΩçsåı [dir_light.h]
// Author : Yamashita Yasuaki
// Date : 2015/5/3
//
//=============================================================================
#ifndef __DLIGHT_H__
#define __DLIGHT_H__

#define LIGHT_NUM 3

//#include "dirLight.hlsli"
//uniform DirLight gDLight[LIGHT_NUM];

uniform float3 gLightDir[LIGHT_NUM];
uniform float  gLightPower[LIGHT_NUM];
uniform float4 gLightCol[LIGHT_NUM];


#endif  // __MATERIAL_H__

// EOF