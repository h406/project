//==============================================================================
//
// iTextureOption[iTextureOption.h]
// Author : Yasuaki Yamashita : 2015/12/24
//
//==============================================================================

#pragma once

#ifndef _ITEXTUREOPTION_H_
#define _ITEXTUREOPTION_H_

#include "app.h"
#include "renderer.h"
#include "texture.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class iTextureOption {
public:
  iTextureOption() : 
    _textureID(0),
    _numU(1),
    _numV(1),
    _animID(0),
    _texPos(0,0),
    _texScl(1.f,1.f)
    {};

  virtual void setNumU(int u) { _numU = u; }
  virtual void setNumV(int v) { _numV = v; }
  virtual void setAnimID(int id) { _animID = id; }

  virtual void setTexPos(const Vec2& texpos) { _texPos = Vec2((1.f / _numU) / getScl().x*texpos.x,(1.f / _numU) / getScl().y*texpos.y); }
  virtual void setTexPos(float x,float y) { setTexPos(Vec2(x,y)); }
  virtual void setTexPosRaw(const Vec2& texpos) { _texPos = texpos; }
  virtual void setTexPosRaw(float x,float y) { _texPos = Vec2(x,y); }

  virtual void setTexSize(const Vec2& texsize) { _texScl = Vec2(1.f / getScl().x*texsize.x,1.f / getScl().y*texsize.y); }
  virtual void setTexSize(float x,float y) { setTexSize(Vec2(x,y)); }
  virtual void setTexScl(const Vec2& texscl) { _texScl = texscl; }
  virtual void setTexScl(float x,float y) { _texScl = Vec2(x,y); }

  const Vec2 getTexScl() const { return _texScl; }
  const Vec2 getTexPos() const { return _texPos; }
  unsigned int getNumU() const { return _numU; }
  unsigned int getNumV() const { return _numV; }
  unsigned int getAnimID() const { return _animID; }

  void setTexture(unsigned int id) { _textureID = id; }
  void setTexture(const char* file) {
    _textureID = App::instance().getRenderer()->getTexture()->createTexture(file);
  }

protected:
  unsigned int _textureID;

  unsigned int _numU;
  unsigned int _numV;
  unsigned int _animID;
  Vec2 _texPos;
  Vec2 _texScl;

  virtual const Vec3& getScl() const = 0;
};

#endif
//EOF