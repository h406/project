//==============================================================================
//
// gauge[gauge.h]
// Author : masato masuda : 2015/10/21
//
//==============================================================================

#pragma once

#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "CreateFunc.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class Gauge : public iLayer2D, public CreateFunc<Gauge> {
public:
  using CreateFunc::create;

  bool init(float width, float height);
  void update();
  void uninit();

  void setRate(float rate){ _rateDest = rate; }
  float getRate(void){ return _rate; }

  void setFlip(bool flip){ _flip = flip ? -1.0f : 1.0f; }

  void setColor(const D3DXCOLOR& color){ _gauge->setColor(color); }

private:

  Sprite2D* _gaugeBase;
  Sprite2D* _gauge;

  float _rate;
  float _rateDest;
  float _flip;
};

#endif
//EOF