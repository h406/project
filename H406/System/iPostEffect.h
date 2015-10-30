//==============================================================================
//
// iPostEffect[iPostEffect.h]
// Author : Yasuaki Yamashita : 2015/10/30
//
//==============================================================================

#pragma once

#ifndef _IPOSTEFFECT_H_
#define _IPOSTEFFECT_H_

class Renderer;
class Sprite2D;

//==============================================================================
// iPostEffect
//------------------------------------------------------------------------------
class iPostEffect {
public:
  virtual bool init() = 0;
  virtual void update() = 0;
  virtual void draw(Renderer *renderer,Sprite2D* sprite) = 0;
};

#endif
//EOF