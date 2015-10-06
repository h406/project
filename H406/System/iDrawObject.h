//==============================================================================
//
// iDrawObject[iDrawObject.h]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

#pragma once

#ifndef _IDRAWOBJECT_H_
#define _IDRAWOBJECT_H_

/*
�\������I�u�W�F�N�g
*/

#include "node.h"
struct VtxShader;

//==============================================================================
// 
//==============================================================================
class iDrawObject : public node {
public:
  virtual void update() = 0;
  virtual void draw(const Renderer* renderer) { UnusedParam(renderer); }
  virtual void uninit() = 0;

protected:
  unsigned int _vtxShaderID;
};

#endif
//EOF