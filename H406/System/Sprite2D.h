//==============================================================================
//
// Sprite2d[Sprite2d.h]
// Author : Yasuaki Yamashita : 2015/10/05
//
//==============================================================================

#pragma once

#ifndef _SPRITE2D_H_
#define _SPRITE2D_H_

#include "iDrawObject.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class Sprite2D : public iDrawObject{
public:
  bool init();
  bool init(const char* file);
  virtual void update();
  void draw(const Renderer* renderer);
  void uninit();

  Sprite2D() {};
  virtual ~Sprite2D() {};
private:

  unsigned int _textureID;

  D3DXCOLOR _color;

};

#endif
//EOF