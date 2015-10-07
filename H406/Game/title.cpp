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


Sprite3D * test = nullptr;
XFileObject* test2 = nullptr;
CameraBace* cam1 = nullptr;

int id = -1;

float rot = D3DX_PI * -0.5f;

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
bool Title::init() {

  test = Sprite3D::create<Sprite3D>("./data/texture/akira000.png");
  test->setSize(36,72);
  this->addChild(test);
  test2 = XFileObject::create<XFileObject>("./data/model/kuma.x");
  this->addChild(test2);

  auto cam = App::instance().getRenderer()->getCamera();
  cam1 = cam->createCamera();

  cam1->setPosP({0,50,-200});
  cam1->setPosR({0,0,0});
  cam->setCurrentCamera(cam1);

  return true;
}

void Title::update() {
  auto input = App::instance().getInput();
  auto effect = App::instance().getRenderer()->getEffect();

  auto pos = test->getPos();

  if(!effect->isExists(id)) {
    id = -1;
  }

  if(input->isPress(VK_INPUT::_3) && id == -1) {
    id = effect->play("test2.efk",pos);
    //effect->setRot(id,{0,0,D3DX_PI * 0.5f});
  }
  if(input->isRelease(VK_INPUT::_4)) {
    App::instance().getSound()->play("./data/sound/se/cursor.wav",false);
  }

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

  if(id != -1) {
    effect->setPos(id,pos);
  }

  if(input->isPress(VK_INPUT::_1)) {
    rot -= 0.1f;
  }
  if(input->isPress(VK_INPUT::_2)) {
    rot += 0.1f;
  }

  //this->setRotY(rot);

  cam1->setPosP({cosf(rot) * 200,50,sinf(rot) * 200});

  char title[255];
  sprintf_s(title,"%d",App::instance().getFps());
  App::instance().setTitle(title);
}

void Title::uninit() {

}

//EOF