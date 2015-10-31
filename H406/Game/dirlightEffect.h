//==============================================================================
//
// dirlightEffect[dirlightEffect.h]
// Author : Yasuaki Yamashita : 2015/10/30
//
//==============================================================================

#pragma once

#ifndef _DIRLIGHT_EFFECT_H_
#define _DIRLIGHT_EFFECT_H_

//==============================================================================
// class
//------------------------------------------------------------------------------
class DirlightEffect : public iPostEffect {
public:
  bool init();
  void update();
  void draw(Renderer *renderer,Sprite2D* sprite);
  void uninit();
};

#endif
//EOF