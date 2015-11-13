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
#include "BaceScene.h"

CameraBace* _camera;

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
bool Title::init() {
  auto camera = App::instance().getRenderer()->getCamera();
  _camera = camera->createCamera();
  _camera->setPosP({0,10,0});
  _camera->setPosR({0,0,0});
  camera->setCamera(_camera, 100);

  return true;
}

void Title::update() {
  static float f = 0;
  f += 0.01f;
  _camera->setPosP({sinf(f) * 500,100,cosf(f) * 500});

  if(App::instance().getInput()->isTrigger(0,VK_INPUT::_1)) {
    BaceScene::instance()->setCurScene(Game::create());
  }
}

void Title::uninit() {
  // ¶¬‚µ‚½ƒJƒƒ‰‚ð‘S‚Äíœ
  App::instance().getRenderer()->getCamera()->releaseCamera(_camera);
}

//EOF