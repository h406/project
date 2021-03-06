//==============================================================================
//
// ShuchuSen[ShuchuSen.cpp]
// Author : Yasuaki Yamashita : 2015/11/25
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "ShuchuSen.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool ShuchuSen::init(const char* imageName) {
  _sprite = Sprite2D::create(imageName);
  _sprite->setSize(1.f,1.f);
  _sprite->setColor(D3DXCOLOR(1.f, 1.f, 1.f, 0.5f));
  this->addChild(_sprite);
  _frame = 0;
  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void ShuchuSen::update() {
  _frame++;
  if(_frame % 5 == 0) {
    _sprite->setRot(rand() % 1000 / 1000.f * D3DX_PI * 2);
    _frame = 0;
  }
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void ShuchuSen::uninit() {

}

//EOF