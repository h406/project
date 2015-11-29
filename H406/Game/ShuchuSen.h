//==============================================================================
//
// ShuchuSen[ShuchuSen.h]
// Author : Yasuaki Yamashita : 2015/11/25
//
//==============================================================================

#pragma once

#ifndef _SHUCHUSEN_H_
#define _SHUCHUSEN_H_

#include "CreateFunc.h"
//==============================================================================
// class
//==============================================================================
class ShuchuSen : public node2d,public CreateFunc<ShuchuSen>{
public:
  using CreateFunc::create;

  bool init(const char* imageName);
  void update();
  void uninit();

private:
  Sprite2D* _sprite;
  unsigned int _frame;
};


#endif
//EOF