//==============================================================================
//
// texSamp0[texSamp0.hlsli]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

#ifndef _TEXSAMP0_H_
#define _TEXSAMP0_H_

// テクスチャ
texture  gTex0;
sampler TexSamp0 = sampler_state {
  Texture = <gTex0>;
  MinFilter = ANISOTROPIC;
  MaxAnisotropy = 8;
  MagFilter = LINEAR;
  MipFilter = LINEAR;
  AddressU = WRAP;
  AddressV = WRAP;
};


#endif
//EOF