//==============================================================================
//
// DrawObject[DrawObject.h]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

#pragma once

#ifndef _DRAWOBJECT_H_
#define _DRAWOBJECT_H_

/*
表示するオブジェクト
*/
class Renderer;

//==============================================================================
// DrawObjectBace
//------------------------------------------------------------------------------
class DrawObjectBace{
public:
  DrawObjectBace();

  void setTexture(unsigned int id);
  void setTexture(const char* file);

  void setColor(const D3DXCOLOR& color) { _color = color; }
  D3DXCOLOR& getColor() { return _color; }

  void setNumU(int u) { _numU = u; }
  void setNumV(int v) { _numV = v; }
  void setAnimID(int id) { _animID = id; }

  void setTexPos(const Vec2& texpos) { _texPos = Vec2((1.f / _numU) / getScl().x*texpos.x,(1.f / _numU) / getScl().y*texpos.y); }
  void setTexPos(float x,float y) { setTexPos(Vec2(x,y)); }
  void setTexPosRaw(const Vec2& texpos) { _texPos = texpos; }
  void setTexPosRaw(float x,float y) { _texPos = Vec2(x,y); }

  void setTexSize(const Vec2& texsize) { _texScl = Vec2(1.f / getScl().x*texsize.x,1.f / getScl().y*texsize.y); }
  void setTexSize(float x,float y) { setTexSize(Vec2(x,y)); }
  void setTexScl(const Vec2& texscl) { _texScl = texscl; }
  void setTexScl(float x,float y) { _texScl = Vec2(x,y); }

  const Vec2 getTexScl() const { return _texScl; }
  const Vec2 getTexPos() const { return _texPos; }
  unsigned int getNumU() const { return _numU; }
  unsigned int getNumV() const { return _numV; }
  unsigned int getAnimID() const { return _animID; }

protected:
  unsigned int _vtxShaderID;
  unsigned int _textureID;

  D3DXCOLOR _color;

  unsigned int _numU;
  unsigned int _numV;
  unsigned int _animID;
  Vec2 _texPos;
  Vec2 _texScl;

  virtual const Vec3& getScl() const = 0;
};

//==============================================================================
// DrawObject
//------------------------------------------------------------------------------
class DrawObject3D : public DrawObjectBace,public node {
protected:
  const Vec3& getScl() const { return _scl; }
};
class DrawObject2D : public DrawObjectBace,public node2d {
protected:
  const Vec3& getScl() const { return _scl; }
};

using DrawObject = DrawObject3D;


#endif
//EOF