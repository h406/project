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

QRreader* _test;

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

  _isEnd = new bool;
  *_isEnd = false;
  _test = new QRreader();
  _QRthread = new thread([](bool* isEnd,QRreader* reader) {
    for(;!(*isEnd);) {
      reader->update();
    }
    SafeDelete(reader);
    SafeDelete(isEnd);
  },_isEnd,_test);

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void SelectScene::update() {
  const auto color = _back->getColor();

  if(_mode == SELECT_MODE::PLAYER1_SELECT || _mode == SELECT_MODE::PLAYER2_SELECT) {
    _back->setColor(color + (D3DXCOLOR(1,1,1,1) - color) * 0.1f);
    _waku->setColor(color + (D3DXCOLOR(1,1,1,1) - color) * 0.1f);
  }
  else {
    _back->setColor(color + (D3DXCOLOR(1,1,1,0) - color) * 0.1f);
    _waku->setColor(color + (D3DXCOLOR(1,1,1,0) - color) * 0.1f);
  }

  switch(_mode) {
  case SELECT_MODE::PLAYER1_SELECT:
    _back->setTexture(_backTex[0]);
    SelectQR(0);
    break;

  case SELECT_MODE::PLAYER1_QR:
    ReadQR(0);
    App::instance().setTitle(_test->getString().c_str());
    break;

  case SELECT_MODE::PLAYER2_SELECT:
    _back->setTexture(_backTex[1]);
    SelectQR(1);
    break;

  case SELECT_MODE::PLAYER2_QR:
    ReadQR(1);
    break;

  case SELECT_MODE::Production:
    if(App::instance().getInput()->isTrigger(0,VK_INPUT::_1)) {
      _mode = (SELECT_MODE)((int)_mode + 1);
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
  (*_isEnd) = true;
  if(_QRthread->joinable()) {
    _QRthread->join();
  }
  SafeDelete(_QRthread);
}

//EOF