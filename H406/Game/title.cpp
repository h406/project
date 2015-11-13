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
#include "SelectScene.h"
#include "BaceScene.h"

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
bool Title::init() {
  auto camera = App::instance().getRenderer()->getCamera();
  _camera = camera->createCamera();
  _camera->setPosP({0,10,0});
  _camera->setPosR({0,0,0});
  camera->setCamera(_camera, 100);
  
  _LogoSprite = Sprite2D::create("./data/texture/title.png");
  _LogoSprite->setSize(1280 * 1.0f,720 *1.0f);
  _LogoSprite->setPos(App::instance().getWindowSize().cx * 0.5f,App::instance().getWindowSize().cy * 0.5f);
  _LogoSprite->setColor(D3DXCOLOR(1,1,1,0));
  this->addChild(_LogoSprite);

  return true;
}

void Title::update() {
  static float f = 0;
  f += 0.01f;
  _camera->setPosP({sinf(f) * 500,100,cosf(f) * 500});

  auto color = _LogoSprite->getColor();
  _LogoSprite->setColor(color + (D3DXCOLOR(1,1,1,1) - color) * 0.01f);

  if(App::instance().getInput()->isTrigger(0,VK_INPUT::_1)) {
    BaceScene::instance()->setCurScene(SelectScene::create());
  }
}

void Title::uninit() {
  // ¶¬‚µ‚½ƒJƒƒ‰‚ð‘S‚Äíœ
  App::instance().getRenderer()->getCamera()->releaseCamera(_camera);
}

//EOF