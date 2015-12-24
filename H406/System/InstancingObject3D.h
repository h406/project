//==============================================================================
//
// InstancingObject3D[InstancingObject3D.h]
// Author : Yasuaki Yamashita : 2015/12/24
//
//==============================================================================

#pragma once

#ifndef _INSTANCINGOBJECT3D_H_
#define _INSTANCINGOBJECT3D_H_

#include "DrawObject.h"
#include "CreateFunc.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class InstancingObject3D :public DrawObject3D, public CreateFunc<InstancingObject3D> {
public:
  using CreateFunc::create;

  bool init() { return true; }
  void update() {}
  void uninit() {}

  void setSizeW(float w) { _worldChenged = true; _scl.x = w; }
  void setSizeH(float h) { _worldChenged = true; _scl.y = h; }
  void setSize(float w,float h) { setSizeW(w); setSizeH(h); }
};

#endif
//EOF