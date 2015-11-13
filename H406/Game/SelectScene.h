//==============================================================================
//
// SelectScene[SelectScene.h]
// Author : Yasuaki Yamashita : 2015/11/13
//
//==============================================================================

#pragma once

#ifndef _SELECTSCENE_H_
#define _SELECTSCENE_H_

#include "CreateFunc.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class SelectScene : public iScene,public CreateFunc<SelectScene> {
public:
  using CreateFunc::create;

  bool init();
  void update();
  void uninit();

private:
  CameraBace* _camera;
};


#endif
//EOF