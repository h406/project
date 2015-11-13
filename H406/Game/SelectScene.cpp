//==============================================================================
//
// SelectScene[SelectScene.cpp]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "SelectScene.h"
#include "game.h"
#include "BaceScene.h"

static float rot = D3DX_PI * 0.5f;

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
bool SelectScene::init() {
  auto camera = App::instance().getRenderer()->getCamera();
  _camera = camera->createCamera();
  _camera->setPosP({-1581.44543f,145.f,692.873657f});
  _camera->setPosR({-1488.66626f,145.f,655.563721f});
  camera->setCamera(_camera, 100);

  return true;
}

void SelectScene::update() {
  Vec3 pos = _camera->getPosP();
  
  if(App::instance().getInput()->isTrigger(0,VK_INPUT::_2)) {
    BaceScene::instance()->setCurScene(Game::create());
  }

  if(App::instance().getInput()->isPress(0,VK_INPUT::LEFT)) {
    pos.x += cosf(rot + D3DX_PI * 0.5f) * 5.f;
    pos.z += sinf(rot + D3DX_PI * 0.5f) * 5.f;
  }
  if(App::instance().getInput()->isPress(0,VK_INPUT::RIGHT)) {
    pos.x += cosf(rot - D3DX_PI * 0.5f) * 5.f;
    pos.z += sinf(rot - D3DX_PI * 0.5f) * 5.f;
  }
  if(App::instance().getInput()->isPress(0,VK_INPUT::UP)) {
    pos.x += cosf(rot) * 5.f;
    pos.z += sinf(rot) * 5.f;
  }
  if(App::instance().getInput()->isPress(0,VK_INPUT::DOWN)) {
    pos.x += cosf(rot + D3DX_PI) * 5.f;
    pos.z += sinf(rot + D3DX_PI) * 5.f;
  }

  if(App::instance().getInput()->isPress(1,VK_INPUT::LEFT)) {
    rot += 0.01f;
  }
  if(App::instance().getInput()->isPress(1,VK_INPUT::RIGHT)) {
    rot -= 0.01f;
  }
  if(App::instance().getInput()->isPress(1,VK_INPUT::UP)) {
    pos.y += 1 * 5.f;
  }
  if(App::instance().getInput()->isPress(1,VK_INPUT::DOWN)) {
    pos.y -= 1 * 5.f;
  }
  //_camera->setPosP(pos);
  //_camera->setPosR(pos + Vec3(cosf(rot)*100.f,0,sinf(rot)*100.f));

}

void SelectScene::uninit() {


}

//EOF