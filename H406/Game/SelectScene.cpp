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
#include "ShuchuSen.h"
#include "ledConnect.h"
#include "BaceScene.h"
#include "playerStatus.h"

#include "../QRDecode/qrdecode.h"

#define _QR_DISABLE__
#define _MODEL (0)

namespace{
  const D3DXCOLOR kPlayerColor[2] = { D3DXCOLOR(0.2f, 0.5f, 1, 0.6f), D3DXCOLOR(1, 0.8f, 0.3f, 0.6f) };
}

ShuchuSen* _s;

XFileObject* _stickLayer = nullptr;
XFileObject* _stickHandle[3] = { nullptr };
XFileObject* _stickBar[3] = { nullptr };

XFileObject* _stickHandlePlayer[2] = { nullptr };
XFileObject* _stickBarPlayer[2] = { nullptr };

CameraBace* _playerCamera = nullptr;

//==============================================================================
// init
//------------------------------------------------------------------------------
bool SelectScene::init() {
  _windowScl = (float)(App::instance().getWindowSize().cx / 1280.f);

  _curSelectPlayer = 0;
  _inputPermit = false;

#if _MODEL

  _stickLayer = XFileObject::create("./data/model/Elephant_bar.x");
  this->addChild(_stickLayer);

  _stickLayer->setVisible(false);
  _stickLayer->setPos(Vec3(200.0f, 0.0f, 0.0f));

  for (int i = 0; i < 3; i++){
    const auto& barStatus = PlayerStatus::kStickBarStatus[(int)i + 1];
    const auto& handStatus = PlayerStatus::kStickHandleStatus[(int)i + 1];

    _stickBar[i] = XFileObject::create(barStatus.fileName);
    _stickLayer->addChild(_stickBar[i]);

    _stickHandle[i] = XFileObject::create(handStatus.fileName);
    _stickLayer->addChild(_stickHandle[i]);

    _stickBar[i]->setScl(Vec3(0.7f, 0.7f, 0.7f));
    _stickHandle[i]->setScl(Vec3(0.7f, 0.7f, 0.7f));
  }
#endif

  auto camera = App::instance().getRenderer()->getCamera();
  _camera = camera->createCamera();
  _camera->setPosP({-1581.44543f,145.f,692.873657f});
  //_camera->setPosR({-1488.66626f,145.f,655.563721f});
  //_camera->setPosP({1226.09229f,395.000000f,-968.748413f});
  _camera->setPosR({0,0,0});
  _camera->setPosP({ -800.f, 145.f, 1000.f });

#if _MODEL
  _camera->setPosP({ 0.f, 100.f, 400.f });
  _camera->setPosR({ 200, 20, 0 });

  _playerCamera = camera->createCamera();
  _playerCamera->setPosP({ 0, 0, 0 });
  _playerCamera->setPosR({ 0, 0, 0 });
#endif
  camera->setCamera(_camera, 100);

  _s = ShuchuSen::create("./data/texture/image.png");
  _s->setSize(1500.f * _windowScl, 1500.f * _windowScl);
  _s->setPos(App::instance().getWindowSize().cx*0.5f, App::instance().getWindowSize().cy*0.5f);
  _s->setVisible(false);
  this->addChild(_s);

  _waku = Sprite2D::create("./data/texture/qr_waku_00.png");
  _waku->setSize((float)App::instance().getWindowSize().cx, (float)App::instance().getWindowSize().cy);
  _waku->setPos(App::instance().getWindowSize().cx*0.5f, App::instance().getWindowSize().cy*0.5f);
  _waku->setColor(D3DXCOLOR(1, 1, 1, 0));
  this->addChild(_waku);

  _back = Sprite2D::create("./data/texture/select_01.png");
  _back->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  _back->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*0.5f);
  _back->setColor(D3DXCOLOR(1,1,1,0));
  this->addChild(_back);

  // てくすちゃーよみこみ
  _backTex[0] = App::instance().getRenderer()->getTexture()->createTexture("./data/texture/select_01.png");
  _backTex[1] = App::instance().getRenderer()->getTexture()->createTexture("./data/texture/select_02.png");
  App::instance().getRenderer()->getTexture()->createTexture("./data/texture/nasi_waku_00.png");

  _mode = SELECT_MODE::PLAYER1_SELECT;
  _select = true;

  _nowReading = Sprite2D::create("./data/texture/now_reading.png");
  _nowReading->setSize(670 * _windowScl, 101 * _windowScl);
  _nowReading->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*0.5f);
  _nowReading->setColor(D3DXCOLOR(1,1,1,0));
  this->addChild(_nowReading);

  // 杖達
//  _stickHandle[0] = XFileObject


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

  _oji = Sprite2D::create("./data/texture/vs_oji.png");
  _oji->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  _oji->setPos(App::instance().getWindowSize().cx*0.5f,-App::instance().getWindowSize().cy*0.5f);
  this->addChild(_oji);

  _oba = Sprite2D::create("./data/texture/vs_oba.png");
  _oba->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  _oba->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*1.5f);
  this->addChild(_oba);

  _vs = Sprite2D::create("./data/texture/vs.png");
  _vs->setSize((float)App::instance().getWindowSize().cx * 0.5f, (float)App::instance().getWindowSize().cy * 0.5f);
  _vs->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*1.5f);
  this->addChild(_vs);

  // サウンドのロード
  // SE
//  App::instance().getSound()->load("./data/sound/se/fanfare.wav");
//  App::instance().getSound()->load("./data/sound/se/rod_select.wav");
  App::instance().getSound()->load("./data/sound/se/qr_ok.wav");
//  App::instance().getSound()->load("./data/sound/se/vs.wav");


  BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::MoveSelect);

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void SelectScene::update() {
  const auto color = _back->getColor();
  const auto wakuColor = _waku->getColor();

  static float f = 0;
  static float _fspeed = 0.02f;
  static int _frame = 0;
  static bool _stickDecision = false;

#if _MODEL
  auto _effect = BaceScene::instance()->getEffect();
  f += _fspeed;
  static float _modelLength = 200.0f;
  static float _lengthSpeed = 1.0f;

  for (int i = 0; i < 3; i++) {
    Vec3 pos = Vec3(cosf(f + D3DX_PI * (2.f / 3.f) * i), 1, sinf(f + D3DX_PI * (2.f / 3.f) * i));
    pos *= _modelLength;
    pos.y = 10.f;

    // pos
    _stickBar[i]->setPos(pos);
    _stickHandle[i]->setPos(pos);

    // rot
    const float rot = atan2(pos.x, pos.z);
    _stickBar[i]->setRotY(rot);
    _stickHandle[i]->setRotY(rot);
  }

  if (_stickBarPlayer[0] != nullptr){
    Vec3 pos = Vec3(cosf(f + D3DX_PI), 1, sinf(f + D3DX_PI));
    pos *= _modelLength;

    // rot
    const float rot = atan2(pos.x, pos.z);
    _stickBarPlayer[0]->setRotY(rot);
    _stickHandlePlayer[0]->setRotY(rot);
  }

#endif

  if(_mode == SELECT_MODE::PLAYER1_SELECT || _mode == SELECT_MODE::PLAYER2_SELECT) {
    _back->setColor(color + (D3DXCOLOR(1,1,1,1) - color) * 0.1f);
    _waku->setColor(wakuColor + (kPlayerColor[_curSelectPlayer] - wakuColor) * 0.1f);
  }
  else {
    const D3DXCOLOR col = kPlayerColor[_curSelectPlayer];
    _back->setColor(color + (D3DXCOLOR(1,1,1,0) - color) * 0.1f);
    _waku->setColor(wakuColor + (D3DXCOLOR(col.r, col.g, col.b, 0) - wakuColor) * 0.1f);
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

  case SELECT_MODE::PLAYER1_DECISION:
  {
#if _MODEL
    _modelLength -= _lengthSpeed; if (_modelLength < 0) _modelLength = 0;
    _fspeed += 0.002f;

    if (_modelLength < 2.f){

      // 杖生成
      const auto& barStatus = PlayerStatus::kStickBarStatus[3];
      const auto& handStatus = PlayerStatus::kStickHandleStatus[3];

      _stickBarPlayer[0] = XFileObject::create(barStatus.fileName);
      this->addChild(_stickBarPlayer[0]);

      _stickHandlePlayer[0] = XFileObject::create(handStatus.fileName);
      this->addChild(_stickHandlePlayer[0]);

      _stickBarPlayer[0]->setScl(Vec3(0.7f, 0.7f, 0.7f));
      _stickHandlePlayer[0]->setScl(Vec3(0.7f, 0.7f, 0.7f));

      _stickBarPlayer[0]->setPos(_stickLayer->getPos());
      _stickHandlePlayer[0]->setPos(_stickLayer->getPos());

      // 杖登場エフェクト
      Vec3 pos = _stickLayer->getPos();
      int id  = _effect->play("GetItemBlue.efk", pos);
      int id2 = _effect->play("get.efk", pos);
      _effect->setEffectScl(id, Vec3(1.f, 1.f, 1.f));
      _effect->setEffectScl(id2, Vec3(5.f, 5.f, 5.f));

      // 集中線
      _s->setVisible(true);

      // 杖消す
      for (int i = 0; i < 3; i++){
        _stickHandle[i]->setVisible(false);
        _stickBar[i]->setVisible(false);
//        _stickDecision = true;
//        _frame = 120;
      }
      // 次のモードへ
      _mode = (SELECT_MODE)((int)_mode + 1);

      // カメラ
      auto camera = App::instance().getRenderer()->getCamera();
      _playerCamera->setPosP({ 150.f, 50.f, 150.f });
      _playerCamera->setPosR({ 200, 50, 0 });
      camera->setCamera(_playerCamera, 60);
    }

#else
    _mode = (SELECT_MODE)((int)_mode + 1);
#endif
  }
    break;

  case SELECT_MODE::PLAYER1_SHOW:
  {
#if _MODEL
    _fspeed -= 0.002f;
    if (_fspeed < 0.02f) _fspeed = 0.02f;

    if (App::instance().getInput()->isTrigger(0, VK_INPUT::_1)) {
      _mode = (SELECT_MODE)((int)_mode + 1);
    }
#else
    _mode = (SELECT_MODE)((int)_mode + 1);
#endif
  }
    break;

  case SELECT_MODE::PLAYER2_SELECT:
    _back->setTexture(_backTex[1]);
    SelectQR(1);
    break;

  case SELECT_MODE::PLAYER2_QR:
    ReadQR(1);
    break;

  case SELECT_MODE::PLAYER2_DECISION:
  {
#if _MODEL
#else
_mode = (SELECT_MODE)((int)_mode + 1);
#endif
  }
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
//    App::instance().getSound()->play("./data/sound/se/system_ok.wav", false);
    App::instance().getSound()->stop("./data/sound/bgm/select_mode.wav");
    BaceScene::instance()->setCurScene(Game::create());
    break;
  }
  _inputPermit = true;
}

//==============================================================================
// selectQR
//------------------------------------------------------------------------------
void SelectScene::SelectQR(int playerID) {
  static float f = D3DX_PI * 0.5f;

  if(App::instance().getInput()->isTrigger(playerID,VK_INPUT::LEFT)) {
    _waku->setTexture("./data/texture/qr_waku_00.png");
    _select = true;
    f = D3DX_PI * 0.5f;
  }
  else if(App::instance().getInput()->isTrigger(playerID,VK_INPUT::RIGHT)) {
    _waku->setTexture("./data/texture/nasi_waku_00.png");
    _select = false;
    f = D3DX_PI* 0.5f;
  }

  if (App::instance().getInput()->isTrigger(playerID, VK_INPUT::_1) && _inputPermit == true) {
    _waku->setTexture("./data/texture/qr_waku_00.png");
    _mode = (SELECT_MODE)((int)_mode + 1 + (_select ? 0 : 1 + 1));
    _select = true;
    f = D3DX_PI;
    _curSelectPlayer++;
    if (_curSelectPlayer > 1) _curSelectPlayer = 1;
    App::instance().getSound()->play("./data/sound/se/qr_ok.wav", false);
    _inputPermit = false;
  }
  f += 0.1f;
  const D3DXCOLOR col = kPlayerColor[_curSelectPlayer];
  _waku->setColor(D3DXCOLOR(col.r, col.g, col.b, min(col.a, sinf(f) * 10.f)));
}

//==============================================================================
// readQR
//------------------------------------------------------------------------------
void SelectScene::ReadQR(int playerID) {
  if(App::instance().getInput()->isTrigger(playerID,VK_INPUT::_1)) {
    _mode = (SELECT_MODE)((int)_mode + 1);

#ifndef _MODEL
    _mode = (SELECT_MODE)((int)_mode + 1);
#endif
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