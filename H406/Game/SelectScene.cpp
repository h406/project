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
#include <thread>

#include "../QRDecode/qrdecode.h"

#define _QR_DISABLE__

Sprite2D* _s;

//==============================================================================
// init
//------------------------------------------------------------------------------
bool SelectScene::init() {
  auto camera = App::instance().getRenderer()->getCamera();
  _camera = camera->createCamera();
  _camera->setPosP({-1581.44543f,145.f,692.873657f});
  //_camera->setPosR({-1488.66626f,145.f,655.563721f});
  //_camera->setPosP({1226.09229f,395.000000f,-968.748413f});
  _camera->setPosR({0,0,0});
  camera->setCamera(_camera, 100);
  
  _back = Sprite2D::create("./data/texture/select1.png");
  _back->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  _back->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*0.5f);
  _back->setColor(D3DXCOLOR(1,1,1,0));
  this->addChild(_back);

  _waku = Sprite2D::create("./data/texture/qr_waku.png");
  _waku->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  _waku->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*0.5f);
  _waku->setColor(D3DXCOLOR(1,1,1,0));
  this->addChild(_waku);

  // ‚Ä‚­‚·‚¿‚á[‚æ‚Ý‚±‚Ý
  _backTex[0] = App::instance().getRenderer()->getTexture()->createTexture("./data/texture/select1.png");
  _backTex[1] = App::instance().getRenderer()->getTexture()->createTexture("./data/texture/select2.png");
  App::instance().getRenderer()->getTexture()->createTexture("./data/texture/nasi_waku.png");

  _mode = SELECT_MODE::PLAYER1_SELECT;
  _select = true;

  _nowReading = Sprite2D::create("./data/texture/now_reading.png");
  _nowReading->setSize(670,101);
  _nowReading->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*0.5f);
  _nowReading->setColor(D3DXCOLOR(1,1,1,0));
  this->addChild(_nowReading);

#ifndef _QR_DISABLE__
  _isEnd = new bool;
  *_isEnd = false;
  _QRreader = new QRreader();
  _QRthread = new thread([](bool* isEnd,QRreader* reader) {
    for(;!(*isEnd);) {
      reader->update();
    }
    SafeDelete(reader);
    SafeDelete(isEnd);
  },_isEnd,_QRreader);
#endif

  _oji = Sprite2D::create("./data/texture/oji.png");
  _oji->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  _oji->setPos(App::instance().getWindowSize().cx*0.5f,-App::instance().getWindowSize().cy*0.5f);
  this->addChild(_oji);

  _oba = Sprite2D::create("./data/texture/oba.png");
  _oba->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  _oba->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*1.5f);
  this->addChild(_oba);

  _vs = Sprite2D::create("./data/texture/vs.png");
  _vs->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  _vs->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*1.5f);
  this->addChild(_vs);

  _s = Sprite2D::create("./data/texture/image.png");
  _s->setSize(1820.f,1820.f);
  _s->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*0.5f);
  this->addChild(_s);

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void SelectScene::update() {
  const auto color = _back->getColor();
  static int f = 0;
  f++;

  if(f % 5 == 0) {
    _s->setRot(rand() % 1000 / 1000.f * D3DX_PI * 2);
  }


  if(_mode == SELECT_MODE::PLAYER1_SELECT || _mode == SELECT_MODE::PLAYER2_SELECT) {
    _back->setColor(color + (D3DXCOLOR(1,1,1,1) - color) * 0.1f);
    _waku->setColor(color + (D3DXCOLOR(1,1,1,1) - color) * 0.1f);
  }
  else {
    _back->setColor(color + (D3DXCOLOR(1,1,1,0) - color) * 0.1f);
    _waku->setColor(color + (D3DXCOLOR(1,1,1,0) - color) * 0.1f);
  }

  if(_mode == SELECT_MODE::PLAYER1_QR || _mode == SELECT_MODE::PLAYER2_QR) {
    static float f = 0;
    f += 0.01f;
    _nowReading->setColor(D3DXCOLOR(1,1,1,1));
    _nowReading->setRotY(f);
  }
  else {
    _nowReading->setColor(D3DXCOLOR(1,1,1,0));
  }

  switch(_mode) {
  case SELECT_MODE::PLAYER1_SELECT:
    _back->setTexture(_backTex[0]);
    SelectQR(0);
    break;

  case SELECT_MODE::PLAYER1_QR:
    ReadQR(0);
#ifndef _QR_DISABLE__
    App::instance().setTitle(_QRreader->getString().c_str());
#endif
    break;

  case SELECT_MODE::PLAYER2_SELECT:
    _back->setTexture(_backTex[1]);
    SelectQR(1);
    break;

  case SELECT_MODE::PLAYER2_QR:
    ReadQR(1);
    break;

  case SELECT_MODE::Production:
    {
      if(App::instance().getInput()->isTrigger(0,VK_INPUT::_1)) {
        _mode = (SELECT_MODE)((int)_mode + 1);
      }
      const Vec2 pos = Vec2(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*0.5f);
      _oji->setPos(_oji->getPos() + (pos - _oji->getPos()) * 0.2f);
      _oba->setPos(_oba->getPos() + (pos - _oba->getPos()) * 0.2f);
      _vs->setPos(_vs->getPos() + (pos - _vs->getPos()) * 0.1f);
    }
    break;

  case SELECT_MODE::MAX:
    BaceScene::instance()->setCurScene(Game::create());
    break;
  }
}

//==============================================================================
// selectQR
//------------------------------------------------------------------------------
void SelectScene::SelectQR(int playerID) {
  static float f = D3DX_PI * 0.5f;

  if(App::instance().getInput()->isTrigger(playerID,VK_INPUT::LEFT)) {
    _waku->setTexture("./data/texture/qr_waku.png");
    _select = true;
    f = D3DX_PI * 0.5f;
  }
  else if(App::instance().getInput()->isTrigger(playerID,VK_INPUT::RIGHT)) {
    _waku->setTexture("./data/texture/nasi_waku.png");
    _select = false;
    f = D3DX_PI* 0.5f;
  }

  if(App::instance().getInput()->isTrigger(playerID,VK_INPUT::_1)) {
    _waku->setTexture("./data/texture/qr_waku.png");
    _mode = (SELECT_MODE)((int)_mode + 1 + (_select ? 0 : 1));
    _select = true;
    f = D3DX_PI;
  }
  f += 0.1f;

  _waku->setColor(D3DXCOLOR(1,1,1,min(1.f,sinf(f) * 10.f)));
}

//==============================================================================
// readQR
//------------------------------------------------------------------------------
void SelectScene::ReadQR(int playerID) {
  if(App::instance().getInput()->isTrigger(playerID,VK_INPUT::_1)) {
    _mode = (SELECT_MODE)((int)_mode + 1);
  }
}


//==============================================================================
// 
//------------------------------------------------------------------------------
void SelectScene::uninit() {

#ifndef _QR_DISABLE__
  (*_isEnd) = true;
  if(_QRthread->joinable()) {
    _QRthread->join();
  }
  SafeDelete(_QRthread);
#endif
}

//EOF