//==============================================================================
//
// ssaoEffect[ssaoEffect.h]
// Author : Yasuaki Yamashita : 2015/10/30
//
//==============================================================================

#pragma once

#ifndef _SSAO_EFFECT_H_
#define _SSAO_EFFECT_H_

//==============================================================================
// class
//------------------------------------------------------------------------------
class SsaoEffect : public iPostEffect {
public:
  bool init();
  void update();
  void draw(Renderer *renderer,Sprite2D* sprite);
  void uninit();

  static void WINAPI makeRayMap(D3DXVECTOR4* pOut,const D3DXVECTOR2*,const D3DXVECTOR2*,void*);

private:
  LPDIRECT3DTEXTURE9       _rayMap;
};

#endif
//EOF