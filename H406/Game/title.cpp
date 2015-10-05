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

  test = Sprite2D::create<Sprite2D>("./data/texture/akira000.png");
  test->setSize(360,720);
  this->addChild(test);

  return true;
}

void Title::update() {
  auto input = App::instance().getInput();

  auto pos = test->getPos();
  if(input->isPress(VK_INPUT::UP)) {
    pos.y -= 1;
  }
  if(input->isPress(VK_INPUT::DOWN)) {
    pos.y += 1;
  }
  if(input->isPress(VK_INPUT::LEFT)) {
    pos.x -= 1;
  }
  if(input->isPress(VK_INPUT::RIGHT)) {
    pos.x += 1;
  }
  test->setPos(pos);

  auto rot = test->getRot().y;
  if(input->isPress(VK_INPUT::_1)) {
    rot -= 0.1f;
  }
  if(input->isPress(VK_INPUT::_2)) {
    rot += 0.1f;
  }
  test->setRotY(rot);

  char title[255];
  sprintf_s(title,"%d",App::instance().getFps());
  App::instance().setTitle(title);
}

void Title::uninit() {

}

//EOF