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
#include "dataManager.h"

#include "../QRDecode/qrdecode.h"

//#define _QR_DISABLE__
#define _SIMPLE_SELECT__
#define _DEBUG_MODEL__

namespace{
  const D3DXCOLOR kPlayerColor[2] = { D3DXCOLOR(0.2f, 0.5f, 1, 0.6f), D3DXCOLOR(1, 0.8f, 0.3f, 0.6f) };
  const Vec3 kTouchColor[2] = { Vec3(80, 170, 255) / 256, Vec3(255, 215, 60) / 256};

  const float kModelLength       = 200.0;
  const float kModelLengthSpeed  = 1.2f;
  const float kFSpeed            = 0.02f;
  const float kFspeedAdd         = 0.0023f;

  const Vec3 kModelPos           = Vec3(250.f,0,0);
  const Vec3 kMainCameraPos      = Vec3(0, 110.f, 75.f);
//  const Vec3 kPlayerCameraOffset = Vec3(110.f, 0.f, 110.f);
  const Vec3 kPlayerCameraOffset = Vec3(100.f, 0.f, 120.f);

  const int kDebugStickID[2]     = { 2, 3 };
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool SelectScene::init() {
  _windowScl = (float)(App::instance().getWindowSize().cx / 1280.f);

  if (DataManager::instance().isInstance()){
    DataManager::instance().uninit();
  }
  DataManager::instance().init();

  _curSelectPlayer = 0;
  _playerStickID[0] = 3;
  _playerStickID[1] = 1;
  _inputPermit = false;

  _modelLength = kModelLength;
  _lengthSpeed = kModelLengthSpeed;
  _fspeed = kFSpeed;
  _frame = 0;
  _gameF = 0;
  _stickDecision = false;
  _touchFrame = -D3DX_PI * 0.5f;
  _touchVsFrame = -D3DX_PI * 0.5f;
  _nowReadingFrame = 0;

  _stickLayer = nullptr;
  for (int i = 0; i < 2; i++){
    _stickHandlePlayer[i] = nullptr;
    _stickBarPlayer[i] = nullptr;
  }
  for (int i = 0; i < 3; i++){
    _stickHandle[i] = nullptr;
    _stickBar[i] = nullptr;
  }

  // 杖生成
  _stickLayer = XFileObject::create("./data/model/Elephant_bar.x");
  this->addChild(_stickLayer);

  _stickLayer->setVisible(false);
  _stickLayer->setPos(kModelPos);

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

  // カメラ
  auto camera = App::instance().getRenderer()->getCamera();
  _camera = camera->createCamera();
//  _camera->setPosP({ 0.f, 100.f, 400.f });
  _camera->setPosP({ 0.f, kMainCameraPos.y, kModelPos.x + kMainCameraPos.z });
  _camera->setPosR({ kModelPos.x, 40, 0 });

  _playerCamera = camera->createCamera();
  _playerCamera->setPosP({ 0, 0, 0 });
  _playerCamera->setPosR({ 0, 0, 0 });
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

  _back = Sprite2D::create("./data/texture/select_001.png");
  _back->setSize((float)App::instance().getWindowSize().cx, (float)App::instance().getWindowSize().cy);
  _back->setPos(App::instance().getWindowSize().cx*0.5f, App::instance().getWindowSize().cy*0.5f);
  _back->setColor(D3DXCOLOR(1, 1, 1, 0));
  this->addChild(_back);

  _backPlayer = Sprite2D::create("./data/texture/select_player_01.png");
  _backPlayer->setSize((float)App::instance().getWindowSize().cx, (float)App::instance().getWindowSize().cy);
  _backPlayer->setPos(App::instance().getWindowSize().cx*0.5f, App::instance().getWindowSize().cy*0.5f);
  _backPlayer->setColor(D3DXCOLOR(1, 1, 1, 0));
  this->addChild(_backPlayer);

  // てくすちゃーよみこみ
  _backTex[0] = App::instance().getRenderer()->getTexture()->createTexture("./data/texture/select_001.png");
  _backTex[1] = App::instance().getRenderer()->getTexture()->createTexture("./data/texture/select_002.png");
  _backTexPlayer[0] = App::instance().getRenderer()->getTexture()->createTexture("./data/texture/select_player_01.png");
  _backTexPlayer[1] = App::instance().getRenderer()->getTexture()->createTexture("./data/texture/select_player_02.png");
  App::instance().getRenderer()->getTexture()->createTexture("./data/texture/nasi_waku_00.png");

  _mode = SELECT_MODE::PLAYER1_SELECT;
  _select = true;

  // now reading
  const Vec2 kNowReadingSize = Vec2(1920.0f, 480.0f) * 0.35f;
  _nowReading = Sprite2D::create("./data/texture/now_reading.png");
  _nowReading->setSize(kNowReadingSize.x * _windowScl, kNowReadingSize.y * _windowScl);
  _nowReading->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*0.5f);
  _nowReading->setColor(D3DXCOLOR(1,1,1,0));
  this->addChild(_nowReading);

  // touch
  const Vec2 kTouchSize = Vec2(400.0f, 140.0f) * 0.8f;
  _touch = Sprite2D::create("./data/texture/touch_00.png");
  _touch->setSize(kTouchSize.x * _windowScl, kTouchSize.y * _windowScl);
  _touch->setPos(App::instance().getWindowSize().cx - ((kTouchSize.x * _windowScl) * 0.65f),App::instance().getWindowSize().cy -((kTouchSize.y * _windowScl) * 0.7f));
  _touch->setColor(D3DXCOLOR(1,1,1,0));
  this->addChild(_touch);

  // touch
#ifndef _SIMPLE_SELECT__
  const Vec2 kSimTouchSize = Vec2(400.0f, 140.0f) * 1.3f;
  _simpleTouch = Sprite2D::create("./data/texture/touch_00.png");
  _simpleTouch->setSize(kSimTouchSize.x * _windowScl, kSimTouchSize.y * _windowScl);
  _simpleTouch->setPos(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy - ((kSimTouchSize.y * _windowScl) * 1.0f));
  _simpleTouch->setColor(D3DXCOLOR(1, 1, 1, 0));
  this->addChild(_simpleTouch);

  _touchSimFrame = -D3DX_PI * 0.5f;;
#else
#endif

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
  _QRreader->clear();
  _QRreader->charSet("9");
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

  // touchVS
  const Vec2 kTouchVsSize = Vec2(906.0f, 140.0f) * 0.5f;
  _touchVs = Sprite2D::create("./data/texture/touch_start.png");
  _touchVs->setSize(kTouchVsSize.x * _windowScl, kTouchVsSize.y * _windowScl);
  _touchVs->setPos(App::instance().getWindowSize().cx*0.5f, App::instance().getWindowSize().cy - ((kTouchVsSize.y * _windowScl) * 0.7f));
  _touchVs->setColor(D3DXCOLOR(1, 1, 1, 0));
  this->addChild(_touchVs);

  // サウンドのロード
  // SE
  App::instance().getSound()->load("./data/sound/se/system_ok.wav");
  App::instance().getSound()->load("./data/sound/se/qr_ok.wav");
  App::instance().getSound()->load("./data/sound/se/item_bomb.wav");

  BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::MoveSelect);

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void SelectScene::update() {
  const auto color = _back->getColor();
  const auto colorPlayer = _backPlayer->getColor();
  const auto wakuColor = _waku->getColor();
  const auto touchColor = _touch->getColor();

  // 杖系
  _gameF += _fspeed;

  for (int i = 0; i < 3; i++) {
    Vec3 pos = Vec3(cosf(_gameF + D3DX_PI * (2.f / 3.f) * i), 1, sinf(_gameF + D3DX_PI * (2.f / 3.f) * i));
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

  // 杖選択終わったら更新されるやつ
  // 1P
  if (_stickBarPlayer[0] != nullptr){
    Vec3 pos = Vec3(cosf(_gameF + D3DX_PI), 1, sinf(_gameF + D3DX_PI));
    pos *= _modelLength;

    // rot
    const float rot = atan2(pos.x, pos.z);
    _stickBarPlayer[0]->setRotY(rot);
    _stickHandlePlayer[0]->setRotY(rot);
  }
  // 2P
  if (_stickBarPlayer[1] != nullptr){
    Vec3 pos = Vec3(cosf(_gameF + D3DX_PI), 1, sinf(_gameF + D3DX_PI));
    pos *= _modelLength;

    // rot
    const float rot = atan2(pos.x, pos.z);
    _stickBarPlayer[1]->setRotY(rot);
    _stickHandlePlayer[1]->setRotY(rot);
  }

  // UI QR or ナシ
#ifndef _SIMPLE_SELECT__
  // UI SimpleTouch
  if (_mode == SELECT_MODE::PLAYER1_SELECT || _mode == SELECT_MODE::PLAYER2_SELECT) {
    _touchSimFrame += 0.01f;
      const auto col = kTouchColor[_curSelectPlayer];
      _simpleTouch->setColor(D3DXCOLOR(col.x, col.y, col.z, min(1.f, sinf(_touchSimFrame * 10) * 15.f)));
  } else {
    _touchSimFrame = -D3DX_PI * 0.5f;
    const auto simColor = _simpleTouch->getColor();
    const auto col = kTouchColor[_curSelectPlayer];
    _simpleTouch->setColor(simColor + (D3DXCOLOR(col.x, col.y, col.z, 0) - simColor) * 0.1f);
  }
#else
  // UI QR or ナシ
  if(_mode == SELECT_MODE::PLAYER1_SELECT || _mode == SELECT_MODE::PLAYER2_SELECT) {
    _back->setColor(color + (D3DXCOLOR(1,1,1,1) - color) * 0.1f);
    _waku->setColor(wakuColor + (kPlayerColor[_curSelectPlayer] - wakuColor) * 0.1f);
  } else {
    const D3DXCOLOR col = kPlayerColor[_curSelectPlayer];
    _back->setColor(color + (D3DXCOLOR(1,1,1,0) - color) * 0.1f);
    _waku->setColor(wakuColor + (D3DXCOLOR(col.r, col.g, col.b, 0) - wakuColor) * 0.1f);
  }
#endif

  // UI 選択中プレイヤー
  if (_mode != SELECT_MODE::Production || _mode != SELECT_MODE::MAX) {
    _backPlayer->setColor(colorPlayer + (D3DXCOLOR(1, 1, 1, 1) - colorPlayer) * 0.1f);
  } else {
    _backPlayer->setColor(colorPlayer + (D3DXCOLOR(1, 1, 1, 0) - colorPlayer) * 0.1f);
  }

  // UI NowReading
  if(_mode == SELECT_MODE::PLAYER1_QR || _mode == SELECT_MODE::PLAYER2_QR) {
    static float f = 0;
    f += 0.01f;
    _nowReadingFrame += 0.01f;
    _nowReading->setColor(D3DXCOLOR(1,1,1,1));
    _nowReading->setRotY(_nowReadingFrame);
  } else {
    _nowReading->setColor(D3DXCOLOR(1,1,1,0));
  }

  // UI Touch
  if (_mode == SELECT_MODE::PLAYER1_SHOW || _mode == SELECT_MODE::PLAYER2_SHOW) {
    if (_fspeed < 0.035f){
      _touchFrame += 0.01f;
      const auto col = kTouchColor[_curSelectPlayer];
      _touch->setColor(D3DXCOLOR(col.x, col.y, col.z, min(1.f, sinf(_touchFrame * 10) * 15.f)));
    }
  } else {
    _touchFrame = -D3DX_PI * 0.5f;
    const auto col = kTouchColor[_curSelectPlayer];
    _touch->setColor(touchColor + (D3DXCOLOR(col.x, col.y, col.z, 0) - touchColor) * 0.1f);
  }

  // モードの処理
  switch(_mode) {
  case SELECT_MODE::PLAYER1_SELECT:
    _back->setTexture(_backTex[0]);
    _backPlayer->setTexture(_backTexPlayer[0]);
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
    _modelLength -= _lengthSpeed; if (_modelLength < 0) _modelLength = 0;
    _fspeed += kFspeedAdd;

    if (_modelLength < 2.f){
      // 杖生成
      CreateStick(0, _playerStickID[0]);

      // 集中線
      _s->setVisible(true);

      // 杖消す
      for (int i = 0; i < 3; i++){
        _stickHandle[i]->setVisible(false);
        _stickBar[i]->setVisible(false);
      }
      // 次のモードへ
      _mode = (SELECT_MODE)((int)_mode + 1);

      // カメラ
      auto camera = App::instance().getRenderer()->getCamera();
      _playerCamera->setPosP({ kModelPos.x - kPlayerCameraOffset.x, 50.f, kPlayerCameraOffset.z });
      _playerCamera->setPosR({ kModelPos.x, 55, 0 });
      camera->setCamera(_playerCamera, 50);
    }
  }
    break;

  case SELECT_MODE::PLAYER1_SHOW:
  {
    _fspeed -= 0.002f;
    if (_fspeed < 0.03f) _fspeed = 0.03f;

    if (App::instance().getInput()->isTrigger(0, VK_INPUT::_1)) {
      _mode = (SELECT_MODE)((int)_mode + 1);

      // カメラ
      auto camera = App::instance().getRenderer()->getCamera();

      // メインカメラ
      _camera->setPosP({ 0.f, kMainCameraPos.y, kModelPos.x + kMainCameraPos.z });
      _camera->setPosR({ -kModelPos.x, 40, 0 });
      camera->setCamera(_camera, 90);

      // モデル
      _stickLayer->setPos(Vec3(-kModelPos.x, 0.0f, 0.0f));
      _gameF = 0;
      _fspeed = kFSpeed;
      _frame = 0;
      _stickDecision = false;
      _modelLength = kModelLength;
      _lengthSpeed = kModelLengthSpeed;

      // 杖表示
      for (int i = 0; i < 3; i++){
        _stickHandle[i]->setVisible(true);
        _stickBar[i]->setVisible(true);
      }

      // 集中線
      _s->setVisible(false);

      // 次のプレイヤー
      _curSelectPlayer++;
      if (_curSelectPlayer > 1) _curSelectPlayer = 1;

      // QRのクリア
#ifndef _QR_DISABLE__
      _QRreader->clear();
      _QRreader->charSet("9");
#endif

      // サウンド
      App::instance().getSound()->play("./data/sound/se/system_ok.wav", false);
    }
  }
    break;

  case SELECT_MODE::PLAYER2_SELECT:
    _back->setTexture(_backTex[1]);
    _backPlayer->setTexture(_backTexPlayer[1]);
    SelectQR(1);
    break;

  case SELECT_MODE::PLAYER2_QR:
    ReadQR(1);
    break;

  case SELECT_MODE::PLAYER2_DECISION:
  {
    _modelLength -= _lengthSpeed; if (_modelLength < 0) _modelLength = 0;
    _fspeed += kFspeedAdd;

    if (_modelLength < 2.f){
      // 杖生成
      CreateStick(1, _playerStickID[1]);

      // 集中線
      _s->setVisible(true);

      // 杖消す
      for (int i = 0; i < 3; i++){
        _stickHandle[i]->setVisible(false);
        _stickBar[i]->setVisible(false);
      }
      // 次のモードへ
      _mode = (SELECT_MODE)((int)_mode + 1);

      // カメラ
      auto camera = App::instance().getRenderer()->getCamera();
      _playerCamera->setPosP({ -(kModelPos.x - kPlayerCameraOffset.x), 50.f, kPlayerCameraOffset.z });
      _playerCamera->setPosR({ -kModelPos.x, 55, 0 });
      camera->setCamera(_playerCamera, 50);
    }
  }
    break;

  case SELECT_MODE::PLAYER2_SHOW:
  {
    _fspeed -= 0.002f;
    if (_fspeed < 0.03f) _fspeed = 0.03f;

    if (App::instance().getInput()->isTrigger(1, VK_INPUT::_1)) {
      _mode = (SELECT_MODE)((int)_mode + 1);

      // カメラ
      auto camera = App::instance().getRenderer()->getCamera();
      _camera->setPosP({ -300.f, 1300.f, 0.f });
      _camera->setPosR({ 0, 0, 0 });
      camera->setCamera(_camera, 60);

      // サウンド
      App::instance().getSound()->play("./data/sound/se/system_ok.wav", false);
    }
  }
    break;

  case SELECT_MODE::Production:
    {
      if(App::instance().getInput()->isTrigger(0,VK_INPUT::_1) || App::instance().getInput()->isTrigger(1,VK_INPUT::_1)) {
        App::instance().getSound()->play("./data/sound/se/system_ok.wav", false);
        _mode = (SELECT_MODE)((int)_mode + 1);
      }
      const Vec2 pos = Vec2(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*0.5f);
      _oji->setPos(_oji->getPos() + (pos - _oji->getPos()) * 0.2f);
      _oba->setPos(_oba->getPos() + (pos - _oba->getPos()) * 0.2f);
      _vs->setPos(_vs->getPos() + (pos - _vs->getPos()) * 0.1f);

      // UI Touch
      const float distance = abs(pos.y - _vs->getPos().y);
      if (distance < 5.0){
        _touchVsFrame += 0.01f;
        _touchVs->setColor(D3DXCOLOR(1, 1, 1, min(1.f, sinf(_touchVsFrame * 10) * 15.f)));
      }
  }
    break;

  case SELECT_MODE::MAX:
#ifndef _DEBUG_MODEL__
    _playerStickID[0] = kDebugStickID[0];
    _playerStickID[1] = kDebugStickID[1];
#endif
    // 杖登録
    DataManager::instance().setPlayerStatus(0, _playerStickID[0]);
    DataManager::instance().setPlayerStatus(1, _playerStickID[1]);

    App::instance().getSound()->stop("./data/sound/bgm/select_mode.wav");
    BaceScene::instance()->setCurScene(Game::create());
    break;
  }
  _inputPermit = true;
}

//==============================================================================
// CreateStick
//------------------------------------------------------------------------------
void SelectScene::CreateStick(int playerID, int sticlID)
{
  if (playerID >= 2 || sticlID >= 4) return;

#ifndef _DEBUG_MODEL__
  _playerStickID[playerID] = kDebugStickID[playerID];
  sticlID = kDebugStickID[playerID];
#endif

  auto _effect = BaceScene::instance()->getEffect();
  const auto& barStatus = PlayerStatus::kStickBarStatus[sticlID];
  const auto& handStatus = PlayerStatus::kStickHandleStatus[sticlID];

  // 生成
  _stickBarPlayer[playerID] = XFileObject::create(barStatus.fileName);
  this->addChild(_stickBarPlayer[playerID]);

  _stickHandlePlayer[playerID] = XFileObject::create(handStatus.fileName);
  this->addChild(_stickHandlePlayer[playerID]);

  _stickBarPlayer[playerID]->setScl(Vec3(0.7f, 0.7f, 0.7f));
  _stickHandlePlayer[playerID]->setScl(Vec3(0.7f, 0.7f, 0.7f));

  _stickBarPlayer[playerID]->setPos(_stickLayer->getPos());
  _stickHandlePlayer[playerID]->setPos(_stickLayer->getPos());

  // 杖登場エフェクト
  Vec3 pos = _stickLayer->getPos();
  int id = 0 , id2 = 0;
  if (playerID == 0){
    id = _effect->play("GetItemBlue.efk", pos);
    id2 = _effect->play("DripGetBlue.efk", pos);
  }else if(playerID == 1){
    id = _effect->play("GetItemYellow.efk", pos);
    id2 = _effect->play("DripGetYellow.efk", pos);
  }

  _effect->setEffectScl(id, Vec3(1.f, 1.f, 1.f));
  _effect->setEffectScl(id2, Vec3(5.f, 5.f, 5.f));

  App::instance().getSound()->play("./data/sound/se/item_bomb.wav", false);
}

//==============================================================================
// selectQR
//------------------------------------------------------------------------------
void SelectScene::SelectQR(int playerID) {
  static float f = D3DX_PI * 0.5f;

  if (App::instance().getInput()->isTrigger(playerID, VK_INPUT::LEFT)) {
    _waku->setTexture("./data/texture/qr_waku_00.png");
    _select = true;
    f = D3DX_PI * 0.5f;
  }
  else if (App::instance().getInput()->isTrigger(playerID, VK_INPUT::RIGHT)) {
    _waku->setTexture("./data/texture/nasi_waku_00.png");
    _select = false;
    f = D3DX_PI* 0.5f;
  }

#ifndef _SIMPLE_SELECT__
  if (App::instance().getInput()->isTrigger(playerID, VK_INPUT::_1) && _inputPermit == true) {
    _select = false;

    // 杖の抽選
    _playerStickID[playerID] = (rand() % 3) + 1;

    // 次のモード
    _mode = (SELECT_MODE)((int)_mode + 1 + (_select ? 0 : 1));

    // 設定初期化
    _waku->setTexture("./data/texture/qr_waku_00.png");
    _select = true;
    _inputPermit = false;
    _nowReadingFrame = 0;
    f = D3DX_PI;

    App::instance().getSound()->play("./data/sound/se/system_ok.wav", false);
  }
#else
  if (App::instance().getInput()->isTrigger(playerID, VK_INPUT::_1) && _inputPermit == true) {
    // 杖の抽選
    if (_select == false){
      _playerStickID[playerID] = (rand() % 3) + 1;
    }

    // 次のモード
    _mode = (SELECT_MODE)((int)_mode + 1 + (_select ? 0 : 1));

    // 設定初期化
    _waku->setTexture("./data/texture/qr_waku_00.png");
    _select = true;
    _inputPermit = false;
    _nowReadingFrame = 0;
    f = D3DX_PI;

    App::instance().getSound()->play("./data/sound/se/system_ok.wav", false);
  }

  f += 0.1f;
  const D3DXCOLOR col = kPlayerColor[_curSelectPlayer];
  _waku->setColor(D3DXCOLOR(col.r, col.g, col.b, min(col.a, sinf(f) * 10.f)));
#endif
}

//==============================================================================
// readQR
//------------------------------------------------------------------------------
void SelectScene::ReadQR(int playerID) {

  if(App::instance().getInput()->isTrigger(playerID,VK_INPUT::_1)) {
    // 戻る
    _mode = (SELECT_MODE)((int)_mode - 1);
    _inputPermit = false;
  }

#ifndef _QR_DISABLE__
  bool isRead = true;

  // 杖選択
  switch (_QRreader->getString().c_str()[0]){
  case '0':
    _playerStickID[playerID] = 1;
    break;
  case '1':
    _playerStickID[playerID] = 2;
    break;
  case '2':
    _playerStickID[playerID] = 3;
    break;
  default:
    isRead = false;
    break;
  }

  if (isRead){
    _QRreader->clear();
    _QRreader->charSet("9");

    _mode = (SELECT_MODE)((int)_mode + 1);
    App::instance().getSound()->play("./data/sound/se/qr_ok.wav", false);
  }
#endif
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