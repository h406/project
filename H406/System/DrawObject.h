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

#include "app.h"
#include "renderer.h"
#include "texture.h"
#include "node.h"
struct VtxShader;

//==============================================================================
// 
//==============================================================================
class DrawObject : public node {
public:
  virtual void update() = 0;
  virtual void draw(const Renderer* renderer) { UnusedParam(renderer); }
  virtual void uninit() = 0;

  void setTexture(unsigned int id) {
    _textureID = id;
  }

  void setTexture(const char* file) {
    _textureID = App::instance().getRenderer()->getTexture()->createTexture(file);
  }
  DrawObject() :_textureID(0) {};
protected:
  unsigned int _vtxShaderID;
  unsigned int _textureID;
};

#endif
//EOF