//==============================================================================
//
// title[title.h]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

#pragma once

#ifndef _TITLE_H_
#define _TITLE_H_

#include "CreateFunc.h"

class CameraBace;

//==============================================================================
// 
//==============================================================================
class Title : public iScene ,public CreateFunc<Title> {
public:
  using CreateFunc::create;

  bool init();
  void update();
  void uninit();

private:
  CameraBace* _camera;
  Sprite2D* _LogoSprite;

  Sprite2D* _touch_start;
};

#endif
//EOF