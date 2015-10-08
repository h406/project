//==============================================================================
//
// Sprite3d[Sprite3d.h]
// Author : Yasuaki Yamashita : 2015/10/05
//
//==============================================================================

#pragma once

#ifndef _SPRITE3D_H_
#define _SPRITE3D_H_

#include "iDrawObject.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class Sprite3D : public iDrawObject {
public:
  bool init();
  bool init(const char* file);
  virtual void update();
  void draw(const Renderer* renderer);
  void uninit();

  void setSizeW(float w) { _worldChenged = true; _scl.x = w; }
  void setSizeH(float h) { _worldChenged = true; _scl.y = h; }
  void setSize(float w,float h) { setSizeW(w); setSizeH(h); }

protected:
  virtual NodeType getNodeType() const { return NodeType::normal3D; }

private:
  unsigned int _textureID;

  D3DXCOLOR _color;

  LPDIRECT3DVERTEXBUFFER9 _vtxBuff;
  LPDIRECT3DVERTEXDECLARATION9 _p3DDec;
};

#endif
//EOF