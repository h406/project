//==============================================================================
//
// iObject[iObject.h]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

#pragma once

#ifndef _IOBJECT_H_
#define _IOBJECT_H_

/*
  表示するオブジェクト
*/

#include "node.h"

class Renderer;
struct VtxShader;

//==============================================================================
// 
//==============================================================================
class iObject : public node {
public:
  virtual void update() = 0;
  virtual void draw(Renderer* renderer) = 0;
  virtual void uninit() = 0;

protected:
  Matrix _mtxWorld;
  VtxShader* _vtxShader;
};


#endif
//EOF