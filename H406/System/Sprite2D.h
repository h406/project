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
class Sprite2D : public DrawObject2D,public CreateFunc<Sprite2D> {
public:
  using CreateFunc::create;

  bool init();
  bool init(const char* file);
  virtual void update();
  void draw(const Renderer* renderer);
  void uninit();

protected:
  virtual NodeType getNodeType() const { return NodeType::normal2D; }

private:

  LPDIRECT3DVERTEXBUFFER9 _vtxBuff;
  LPDIRECT3DVERTEXDECLARATION9 _p2DDec;
};

#endif
//EOF