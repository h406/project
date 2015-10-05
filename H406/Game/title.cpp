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

Sprite2D * test = nullptr;

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
bool Title::init() {

  test = Sprite2D::create<Sprite2D>();
  test->setScl({100,100,1});
  this->addChild(test);

  return true;
}

void Title::update() {
  auto input = App::instance().getInput();

  if(input->isPress(VK_INPUT::UP)) {
    App::instance().exit();
  }

  char title[255];
  sprintf_s(title,"%d",App::instance().getFps());
  App::instance().setTitle(title);
}

void Title::uninit() {

}

//EOF