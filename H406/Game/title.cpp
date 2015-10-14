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

namespace {

  const int numx = 12;
  const int numy = 12;
}


Sprite3D * akr = nullptr;
CameraBace* cam1 = nullptr;
CameraBace* cam2 = nullptr;
MeshField* field = nullptr;
Sprite3D* fieldMap[numx][numy] = {nullptr};

Effect* effect = nullptr;
Vec3 moveVec(0,0,0);

int id = -1;

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
bool Title::init() {

  akr = Sprite3D::create("./data/texture/akira000.png");
  akr->setSize(36,72);
  this->addChild(akr);

  auto cam = App::instance().getRenderer()->getCamera();
  cam1 = cam->createCamera();
  cam1->setPosP({0,600,-900});
  cam1->setPosR({0,0,-200});
  cam->setCamera(cam1);

  cam2 = cam->createCamera();
  cam2->setPosP({0,0,0});
  cam2->setPosR({0,0,0});

  field = MeshField::create(1, 1, 1000.f, 1000.f);
  field->setColor(D3DXCOLOR(0,0,0,1));
  this->addChild(field);

  effect = Effect::create();
  effect->setScl(Vec3(10,10,10));
  this->addChild(effect);
  
  const Vec2 bordSize = Vec2(1000 / (float)numx,1000 / (float)numy);

  for(int x = 0; x < numx; x++) {
    for(int y = 0; y < numy; y++) {
      fieldMap[x][y] = Sprite3D::create();
      fieldMap[x][y]->setSize(bordSize.x - 5,bordSize.y - 5);
      fieldMap[x][y]->setPos(Vec3(x * bordSize.x - 1000.f * 0.5f + bordSize.x * 0.5f,00.1f,y * bordSize.y - 1000.f * 0.5f + bordSize.y * 0.5f));
      fieldMap[x][y]->setRotX(D3DX_PI * 0.5f);
      this->addChild(fieldMap[x][y]);
    }
  }

  return true;
}

void Title::update() {
  auto input = App::instance().getInput();
  auto& pos = akr->getPos();

  Vec3 moveDest(0,0,0);

  if(input->isPress(0, VK_INPUT::UP)) {
    moveDest.z += 5;
  }
  if(input->isPress(0, VK_INPUT::DOWN)) {
    moveDest.z -= 5;
  }
  if(input->isPress(0, VK_INPUT::LEFT)) {
    moveDest.x -= 5;
  }
  if(input->isPress(0, VK_INPUT::RIGHT)) {
    moveDest.x += 5;

    App::instance().createNextScene<Title>();
  }
  moveVec += (moveDest - moveVec) * 0.1f;
  akr->setPos(pos + moveVec);

  int idx = int((pos.x + 500) / (1000 / (float)numx));
  int idy = int((pos.z + 500) / (1000 / (float)numy));

  if(idx >= 0 && idx < numy && idy >= 0 && idy < numy) {
    fieldMap[idx][idy]->setColor(D3DXCOLOR(0,0,1,1));
  }

  cam2->setPosP(pos + Vec3(0,20, -100));
  cam2->setPosR(pos);

  if(input->isTrigger(1, VK_INPUT::_1)) {
    App::instance().getRenderer()->getCamera()->setCamera(cam1,10);
    effect->setPos(pos);

    effect->stop(id);
    id = effect->play("test2.efk",Vec3(0,0,0));
    
  }
  if(input->isTrigger(1, VK_INPUT::_2)) {

    App::instance().getRenderer()->getCamera()->setCamera(cam2,10);
  }
}

void Title::uninit() {

}

//EOF