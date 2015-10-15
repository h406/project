//==============================================================================
//
// title[title.cpp]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "title.h"
#include "game.h"

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
bool Title::init() {
  auto size = App::instance().getWindowSize();
  _BGSprite = Sprite2D::create("./data/texture/tx_grass1.tga");
  _BGSprite->setSize(size.cx,size.cy);
  _BGSprite->setPos(size.cx * 0.5f, size.cy * 0.5f, 0);
  this->addChild(_BGSprite);

  return true;
}

void Title::update() {
  if(App::instance().getInput()->isTrigger(0,VK_INPUT::_1)) {
    App::instance().createNextScene<Game>();
  }
}

void Title::uninit() {

}

//EOF