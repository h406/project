//==============================================================================
//
// Sprite3DAdditive[Sprite3DAdditive.h]
//
// Author : Masato Masuda : 2015/12/14
//
//==============================================================================

#pragma once

#ifndef _SPRITE3D_SUBTRACTIVE_H_
#define _SPRITE3D_SUBTRACTIVE_H_

#include "DrawObject.h"
#include "CreateFunc.h"
#include "Sprite3DAdditive.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class Sprite3DAdditive : public DrawObject, public CreateFunc<Sprite3DAdditive> {
public:
  bool init();
  bool init(const char* file);
  virtual void update();
  void draw(const Renderer* renderer);
  void uninit();

  using CreateFunc::create;

  void setSizeW(float w) { _worldChenged = true; _scl.x = w; }
  void setSizeH(float h) { _worldChenged = true; _scl.y = h; }
  void setSize(float w,float h) { setSizeW(w); setSizeH(h); }

protected:
  virtual NodeType getNodeType() const { return NodeType::normal3D; }

private:

  LPDIRECT3DVERTEXBUFFER9 _vtxBuff;
  LPDIRECT3DVERTEXDECLARATION9 _p3DDec;
};

#endif
//EOF