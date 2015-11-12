//==============================================================================
//
// fxaa[fxaa.h]
// Author : Yasuaki Yamashita : 2015/11/12
//
//==============================================================================

#pragma once

#ifndef _FXAA_H_
#define _FXAA_H_

//==============================================================================
// class
//------------------------------------------------------------------------------
class FxaaEffect : public iPostEffect {
public:
  bool init();
  void update();
  void draw(Renderer *renderer,Sprite2D* sprite);
  void uninit();
};


#endif
//EOF