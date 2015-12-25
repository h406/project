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

#include "node.h"
#include "node2d.h"
#include "iColorOption.h"
#include "iTextureOption.h"

//==============================================================================
// DrawObjectBace
//------------------------------------------------------------------------------
class DrawObjectBace{
public:
  DrawObjectBace() : _vtxShaderID(0) {};

protected:
  unsigned int _vtxShaderID;

};

//==============================================================================
// DrawObject
//------------------------------------------------------------------------------
class DrawObject3D :
  public DrawObjectBace,
  public iTextureOption,
  public iColorOption,
  public node {
protected:
  const Vec3& getScl() const { return _scl; }
};

class DrawObject2D :
  public DrawObjectBace,
  public iTextureOption,
  public iColorOption,
  public node2d {
protected:
  const Vec3& getScl() const { return _scl; }
};

using DrawObject = DrawObject3D;


#endif
//EOF