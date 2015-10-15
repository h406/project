//==============================================================================
//
// Sprite2d[Sprite2d.h]
// Author : Yasuaki Yamashita : 2015/10/05
//
//==============================================================================

#pragma once

#ifndef _SPRITE2D_H_
#define _SPRITE2D_H_

#include "DrawObject.h"
#include "CreateFunc.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class Sprite2D : public DrawObject,public CreateFunc<Sprite2D> {
public:
  using CreateFunc::create;

  bool init();
  bool init(const char* file);
  virtual void update();
  void draw(const Renderer* renderer);
  void uninit();

  void setSizeW(float w) { _worldChenged = true; _scl.x = w; }
  void setSizeH(float h) { _worldChenged = true; _scl.y = h; }
  void setSize(float w,float h) { setSizeW(w); setSizeH(h); }
  void setColor(const D3DXCOLOR& color) { _color = color; }

  void setNumU(int u) { _numU = u; }
  void setNumV(int v) { _numV = v; }
  void setAnimID(int id) { _animID = id; }

protected:
  virtual NodeType getNodeType() const { return NodeType::normal2D; }

private:

  void updateWorldMtx();

  unsigned int _numU;
  unsigned int _numV;
  unsigned int _animID;
  Vec2 _texPos;

  D3DXCOLOR _color;

  LPDIRECT3DVERTEXBUFFER9 _vtxBuff;
  LPDIRECT3DVERTEXDECLARATION9 _p2DDec;
};

#endif
//EOF