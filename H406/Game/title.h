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
  Sprite2D* _BGSprite;
};

#endif
//EOF