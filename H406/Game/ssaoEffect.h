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
};

#endif
//EOF