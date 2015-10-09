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
CameraBace* cam2 = nullptr;

Effect* effect = nullptr;
MeshField* field = nullptr;

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
  cam1->setPosP({0,100,-200});
  cam1->setPosR({0,0,0});
  cam->setCamera(cam1);

  cam2 = cam->createCamera();
  cam2->setPosP({0,300,-900});
  cam2->setPosR({0,0,0});


  effect = Effect::create<Effect>();
  effect->setScl(Vec3(10,10,10));
  this->addChild(effect);

  field = MeshField::create<MeshField>(50, 50, 1000.f, 1000.f);
  field->setTexture("./data/texture/tx_grass1.tga");
  this->addChild(field);

  return true;
}

void Title::update() {
  auto input = App::instance().getInput();
  auto pos = effect->getPos();


  if(input->isRelease(VK_INPUT::_3)) {
    id = effect->play("test2.efk",Vec3(0,0,0));
    App::instance().getRenderer()->getCamera()->setCamera(cam1,30);
  }

  if(input->isRelease(VK_INPUT::_4)) {
    App::instance().getSound()->play("./data/sound/se/cursor.wav",false);

    App::instance().getRenderer()->getCamera()->setCamera(cam2,30);
  }

  if(input->isPress(VK_INPUT::UP)) {
    pos.y += 1;
  }
  if(input->isPress(VK_INPUT::DOWN)) {
    pos.y -= 1;
  }
  if(input->isPress(VK_INPUT::LEFT)) {
    pos.x -= 1;
  }
  if(input->isPress(VK_INPUT::RIGHT)) {
    pos.x += 1;
  }
  effect->setPos(pos);

  if(input->isPress(VK_INPUT::_1)) {
    rot -= 0.1f;
  }
  if(input->isPress(VK_INPUT::_2)) {
    rot += 0.1f;
  }

  effect->setRotZ(rot);

  //cam1->setPosP({cosf(rot) * 200,50,sinf(rot) * 200});

  char title[255];
  sprintf_s(title,"%d",App::instance().getFps());
  App::instance().setTitle(title);
}

void Title::uninit() {

}

//EOF