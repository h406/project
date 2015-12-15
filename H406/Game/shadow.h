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

  void setOwner(XFileObject* owner){ _owner = owner; }

private:

  // âe
  Sprite3DSubtractive* _sprite;

  // éùÇøéÂ
  XFileObject* _owner;
};


#endif
//EOF