//==============================================================================
//
// staticStage[staticStage.h]
// Author : Yasuaki Yamashita : 2015/10/19
//
//==============================================================================

#pragma once

#ifndef _STATICSTAGE_H_
#define _STATICSTAGE_H_

#include "CreateFunc.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class StaticStage : public iLayer,public CreateFunc<StaticStage> {
public:
  using CreateFunc::create;

  bool init();
  void update();
  void uninit();
};

#endif
//EOF