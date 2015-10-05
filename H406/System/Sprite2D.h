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
class Sprite2D : public iDrawObject {
public:
  bool init();
  bool init(const char* file);
  virtual void update();
  void draw(const Renderer* renderer);
  void uninit();

  void setSizeW(float w) { _worldChenged = true; _scl.x = w; }
  void setSizeH(float h) { _worldChenged = true; _scl.y = h; }
  void setSize(float w,float h) { setSizeW(w); setSizeH(h); }

private:

  void updateWorldMtx();

  unsigned int _textureID;

  unsigned int _numU;
  unsigned int _numV;
  unsigned int _animID;
  Vec2 _texPos;

  D3DXCOLOR _color;

};

#endif
//EOF