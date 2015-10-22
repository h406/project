//==============================================================================
//
// gauge[gauge.cpp]
// Author : masato masuda : 2015/10/21
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "gauge.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool Gauge::init(float width,float height)
{
  _size = Vec2(width,height);
  _gauge = Sprite2D::create();
  _gauge->setSize(width,height);
  this->addChild(_gauge);

  _rate = 0.0f;
  _flip = 1.0f;

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void Gauge::update(void)
{
  _rate += (_rateDest - _rate) * 0.1f;

  _gauge->setPosX(((_rate * _size.x * 0.5f) * _flip) - ((_size.x * 0.5f) * _flip));
  _gauge->setSizeW(_rate * _size.x);

  _gauge->setTexScl(_rate, 1.f);
  _gauge->setTexPosRaw(1 - _rate * ((_flip - 1) * -0.5f),0);
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void Gauge::uninit(void)
{
}

//EOF
