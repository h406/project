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


static float f =  D3DX_PI * 0.5f;

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
  _LogoSprite->setPos(App::instance().getWindowSize().cx * 0.5f,App::instance().getWindowSize().cy * 0.45f);
  _LogoSprite->setColor(D3DXCOLOR(1,1,1,0));
  this->addChild(_LogoSprite);

  _touch_start = Sprite2D::create("./data/texture/touch_start.png");
  _touch_start->setSize(906,140);
  _touch_start->setPos(App::instance().getWindowSize().cx * 0.5f,App::instance().getWindowSize().cy * 0.85f);
  this->addChild(_touch_start);

  f = -D3DX_PI * 0.5f;

  // サウンドロード
  App::instance().getSound()->load("./data/sound/se/system_ok.wav");

  return true;
}

void Title::update() {
  f += 0.01f;
  _camera->setPosP({sinf(f) * 500,100,cosf(f) * 500});

  auto color = _LogoSprite->getColor();
  _LogoSprite->setColor(color + (D3DXCOLOR(1,1,1,1) - color) * 0.01f);

  _touch_start->setColor(D3DXCOLOR(1,1,1,min(1.f,sinf(f*10) * 10.f)));

  if(App::instance().getInput()->isRelease(0,VK_INPUT::_1)) {
    BaceScene::instance()->setCurScene(SelectScene::create());
    App::instance().getSound()->play("./data/sound/se/system_ok.wav", false);
  }
}

void Title::uninit() {
  // 生成したカメラを全て削除
  App::instance().getRenderer()->getCamera()->releaseCamera(_camera);
}

//EOF