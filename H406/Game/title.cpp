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
XFileObject* test2 = nullptr;

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
bool Title::init() {
  test2 = XFileObject::create<XFileObject>("./data/model/kuma.x");
  this->addChild(test2);
  
  auto cam = App::instance().getRenderer()->getCamera();
  auto cam1 = cam->createCamera();
  cam1->setPosP({0,100,-100});
  cam1->setPosR({0,0,0});
  cam->setCurrentCamera(cam1);

  test = Sprite2D::create<Sprite2D>("./data/texture/akira000.png");
  test->setSize(36,72);
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