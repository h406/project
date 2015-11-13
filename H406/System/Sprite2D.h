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

  const D3DXCOLOR& getColor() const { return _color; }
  void setColor(const D3DXCOLOR& color) { _color = color; }

  void setNumU(int u) { _numU = u; }
  void setNumV(int v) { _numV = v; }
  void setAnimID(int id) { _animID = id; }

  void setTexPos(const Vec2& texpos) { _texPos = Vec2((1.f / _numU) / _scl.x*texpos.x,(1.f / _numU) / _scl.y*texpos.y); }
  void setTexPos(float x,float y) { setTexPos(Vec2(x,y)); }
  void setTexPosRaw(const Vec2& texpos) { _texPos = texpos; }
  void setTexPosRaw(float x, float y) { _texPos = Vec2(x,y); }

  void setTexSize(const Vec2& texsize) { _texScl = Vec2(1.f / _scl.x*texsize.x,1.f / _scl.y*texsize.y); }
  void setTexSize(float x,float y) { setTexSize(Vec2(x,y)); }
  void setTexScl(const Vec2& texscl) { _texScl = texscl; }
  void setTexScl(float x,float y) { _texScl = Vec2(x,y); }

protected:
  virtual NodeType getNodeType() const { return NodeType::normal2D; }

private:
  unsigned int _numU;
  unsigned int _numV;
  unsigned int _animID;
  Vec2 _texPos;
  Vec2 _texScl;

  D3DXCOLOR _color;

  LPDIRECT3DVERTEXBUFFER9 _vtxBuff;
  LPDIRECT3DVERTEXDECLARATION9 _p2DDec;
};

#endif
//EOF