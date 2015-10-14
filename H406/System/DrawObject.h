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
// 
//==============================================================================
class DrawObject : public node {
public:
  virtual void update() = 0;
  virtual void draw(const Renderer* renderer) { UnusedParam(renderer); }
  virtual void uninit() = 0;

  void setTexture(unsigned int id);
  void setTexture(const char* file);

  DrawObject();
protected:
  unsigned int _vtxShaderID;
  unsigned int _textureID;
};

#endif
//EOF