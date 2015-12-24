//==============================================================================
//
// shadow[shadow.h]
// Author : masato masuda : 2015/12/15
//
//==============================================================================
#pragma once

#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "CreateFunc.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class Shadow : public iLayer, public CreateFunc<Shadow> {
public:
  using CreateFunc::create;

  bool init();
  void update();
  void uninit();

  void setOwner(node* owner) { _owner = owner; }
  void setColor(const D3DXCOLOR& col);
  void setOffsetY(float y){ _offsetY = y; }

private:

  // âe
//  Sprite3DSubtractive* _sprite;
  Sprite3D* _sprite;

  // éùÇøéÂ
  node* _owner;

  float _offsetY;
};


#endif
//EOF