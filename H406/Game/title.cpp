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
#include "ledConnect.h"
#include "BaceScene.h"

static float f =  D3DX_PI * 0.5f;

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
bool Title::init() {
  auto camera = App::instance().getRenderer()->getCamera();
  _windowScl = (float)(App::instance().getWindowSize().cx / 1280.f);

  _camera = camera->createCamera();
  _camera->setPosP({0,10,0});
  _camera->setPosR({0,0,0});
  camera->setCamera(_camera, 100);

  const Vec2 kLOGO_SIZE = Vec2((float)App::instance().getWindowSize().cx, (float)App::instance().getWindowSize().cy) * 0.75f;
  _LogoSprite = Sprite2D::create("./data/texture/title.png");
  _LogoSprite->setSize(kLOGO_SIZE.x, kLOGO_SIZE.y);
  _LogoSprite->setPos(App::instance().getWindowSize().cx * 0.5f,App::instance().getWindowSize().cy * 0.4f);
  _LogoSprite->setColor(D3DXCOLOR(1,1,1,0));
  this->addChild(_LogoSprite);

  const Vec2 kTOUCH_START_SIZE = Vec2(906.f, 140.f) * 0.75f;
  _touch_start = Sprite2D::create("./data/texture/touch_start.png");
  _touch_start->setSize(kTOUCH_START_SIZE.x * _windowScl, kTOUCH_START_SIZE.y * _windowScl);
  _touch_start->setPos(App::instance().getWindowSize().cx * 0.5f,App::instance().getWindowSize().cy * 0.87f);
  this->addChild(_touch_start);

  f = -D3DX_PI * 0.5f;

  // サウンドロード
  // BGM
  App::instance().getSound()->play("./data/sound/bgm/select_mode.wav", true);
  // SE
  App::instance().getSound()->load("./data/sound/se/system_ok.wav");


  BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::MoveTitle);

  return true;
}

void Title::update() {
  f += 0.01f;
  _camera->setPosP({sinf(f) * 500,100,cosf(f) * 500});

  auto color = _LogoSprite->getColor();
  _LogoSprite->setColor(color + (D3DXCOLOR(1,1,1,1) - color) * 0.03f);

  _touch_start->setColor(D3DXCOLOR(1,1,1,min(1.f,sinf(f*10) * 10.f)));

  if(App::instance().getInput()->isRelease(0,VK_INPUT::_1)) {
    BaceScene::instance()->setCurScene(SelectScene::create());
    App::instance().getSound()->play("./data/sound/se/system_ok.wav", false);
  }

  if(App::instance().getInput()->isRelease(0,VK_INPUT::_2)) {
//    BaceScene::instance()->getLedConnect()->sendText("ああああああああああああああああああ");
    float a = 0.3f;
    BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::ShowGauge, &a);
  }
}

void Title::uninit() {
  // 生成したカメラを全て削除
  App::instance().getRenderer()->getCamera()->releaseCamera(_camera);
}

//EOF